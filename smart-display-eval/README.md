# Smart Display Eval

An automated stand‑in for the **manual QA tester** who sits in front of a smart‑TV
device, drives it with a remote, and watches the screen for problems.

The app **presses remote buttons, navigates apps, watches the display and listens
to the audio in near real time**, and decides:

- Is the screen showing **UI motion** (menus scrolling, transitions) or **video
  playback**? The two are judged differently.
- Is on‑screen motion **fluid**, or does it **stutter / freeze / judder**?
- During video, are there **artifacts** — **macroblocking**, **low
  resolution / softness**, **audio loss**, **AV‑sync** drift, **black frames**?
- **When** did each problem happen? Every finding is timestamped.

The first supported target is the **Amazon Fire TV Stick**.

> **Why:** this is built to be the **evaluation / verification layer for an
> agentic workflow** that automates output checking of display devices. An agent
> decides *what* to do (open an app, scroll, play a title); this harness provides
> the *ground‑truth observation and grading* — a structured, timestamped,
> machine‑readable report with a 0–100 score.

---

## How it is used physically

```
 ┌──────────────┐   ADB over Wi‑Fi (keyevents, am start)   ┌───────────────┐
 │  Android app │ ───────────────────────────────────────▶ │  Fire TV Stick │
 │  (this repo) │                                           │   + TV / HDMI  │
 │              │   camera / HDMI‑capture sees the screen   │                │
 │   📷  🎙️     │ ◀──────────────────────────────────────── │   🖥️  🔊       │
 └──────────────┘   mic / line‑in hears the audio           └───────────────┘
```

- **Control**: the phone talks ADB directly to the Fire TV over the local network
  (no host `adb` server needed) and injects remote key events.
- **Observation**: the phone camera is pointed at the TV (or an HDMI→USB capture
  dongle feeds frames); the microphone (or a line/HDMI audio tap) provides audio
  level. Both are reduced to a small per‑frame representation the analyzers use.

This mirrors exactly what a human tester does: **act, watch, listen, judge** —
but automatically and repeatably.

---

## Architecture

The project is intentionally split so the hard part (the analysis) is
hardware‑free and fully testable.

```
smart-display-eval/
├── core/   Pure-Kotlin (JVM), dependency-free analysis + eval engine.
│           No Android, no camera, no ADB — unit-testable in isolation.
└── app/    Android (Jetpack Compose) front end: CameraX capture,
            microphone audio, ADB-over-TCP control, and the UI.
```

### `core/` — the analysis & eval engine

| Area | Type(s) | What it does |
|------|---------|--------------|
| Frame model | `Frame`, `AudioSample` | Grayscale luma plane + audio level per captured frame. |
| Image ops | `ImageOps` | Edge density, luma entropy, Laplacian sharpness, DCT‑grid **blockiness**, and **global‑motion coherence** (rigid‑shift search). |
| Scene | `SceneClassifier` → `SceneType` | Classifies each frame `STATIC` / `UI_MOTION` / `VIDEO`. |
| Artifacts | `MacroblockDetector`, `SharpnessAnalyzer`, `AudioLossDetector`, `BlackFrameDetector` | Per‑frame, near‑real‑time detectors emitting timestamped `ArtifactEvent`s. |
| Fluidity | `FluidityAnalyzer` | Freezes/stalls, judder and a 0–100 smoothness score. |
| AV sync | `AvSyncAnalyzer` | Audio vs. video onset‑envelope cross‑correlation → offset in ms. |
| Pipeline | `FrameAnalysisPipeline` | Runs every analyzer over the frame stream, one frame at a time. |
| Scenario | `Scenario`, `Step`, `StepAction`, `Expectation`, `RemoteKey` | Declarative test definition. |
| Runner | `ScenarioRunner`, `RunnerListener` | Drives controller + frame source through the pipeline and checks expectations. |
| Report | `EvalReport`, `JsonWriter` | Per‑step pass/fail, artifact timeline, 0–100 `score()`, JSON output. |
| Sim | `SyntheticFrames`, `ScriptedFrameSource`, `RecordingRemoteController`, `demo.DemoCli` | Hardware‑free generators + a headless demo of the whole pipeline. |

### `app/` — the Android front end

| Component | Role |
|-----------|------|
| `AdbRemoteController` | Implements `RemoteController` using [`dadb`](https://github.com/mobile-dev-inc/dadb) to send `input keyevent` / `am start` / `input text` to the Fire TV over TCP. |
| `CameraFrameSource` | Implements `FrameSource`; downsamples CameraX YUV frames to the core `Frame`. |
| `AudioLevelProbe` | Streams microphone RMS/peak as `AudioSample`s. |
| `EvalViewModel` | Runs a scenario off the UI thread, surfaces live progress + final report. |
| `FireTvScenarios` | Ready‑made scenarios (home‑navigation fluidity, browse & play). |
| `EvalScreen` (Compose) | Camera preview, connection, scenario picker, live status, scored report. |

---

## How the analysis works

All analyzers operate on a small grayscale luma plane (default 160×90) plus a
scalar audio level per frame — cheap enough to run per frame on device.

- **Scene classification** blends three cues: **global‑motion coherence** (does
  the change reduce to one rigid shift? → scrolling UI), **luma entropy**
  (natural video is high‑entropy), and **edge density** (crisp graphics/text →
  UI). Little change → `STATIC`.
- **Macroblocking** compares luma discontinuity **on** the 8×8 codec grid vs.
  **inside** blocks. Compression blocking creates periodic step edges on the
  grid, pushing the ratio above a learned baseline.
- **Low resolution / softness** tracks the **variance of the Laplacian**
  (high‑frequency detail) against a decaying peak; a sustained drop = a
  resolution/bitrate downshift or soft decode.
- **Audio loss** flags sustained near‑silence **while the picture is actively
  moving** (so genuinely quiet moments aren't false‑flagged).
- **Fluidity** finds **freezes** during video (motion collapses to ~0 shortly
  after real motion) and folds cadence irregularity (**judder**) into a
  smoothness score.
- **AV sync** cross‑correlates the audio‑level onset envelope with the
  visual‑change onset envelope; the lag that maximizes correlation is the
  offset (positive = audio lags the picture).

Detectors are conservative (baselines, minimum durations, scene gating) to keep
false positives low, and every event carries a `timestampMs`, `severity` and
`confidence`.

---

## The eval model (for agentic grading)

A `Scenario` is an ordered list of `Step`s. Each step performs an action
(`PressKey`, `LaunchApp`, `InputText`, `Idle`), observes for `observeMs`, then
checks `Expectation`s:

- `SceneShouldBe(type, minFraction)`
- `MinFluidity(min)`
- `NoArtifacts(types, maxSeverity)`
- `MaxAvSyncMs(maxAbsMs)`

Running it produces an `EvalReport`:

- per‑step **pass/fail** with the reason for each expectation,
- the full **artifact timeline**,
- fluidity and AV‑sync summaries,
- a single **0–100 `score()`** (blend of step pass‑rate, fluidity, and an
  artifact penalty),
- `toJson()` for storage / automated grading.

An agent drives *what* to test; this report is the *objective grade*, suitable
for regression tracking or as a reward/eval signal.

---

## Fire TV Stick setup

1. On the Fire TV: **Settings → My Fire TV → Developer Options → ADB debugging = ON**
   (also enable *Apps from Unknown Sources* if you need to sideload the eval app onto a Fire device).
2. Find the Fire TV IP: **Settings → My Fire TV → About → Network**.
3. Put the phone/tablet on the **same Wi‑Fi network**.
4. In the app, enter the IP and tap **Connect**. **Accept the ADB authorization
   prompt** that appears on the TV the first time.
5. Point the phone camera at the TV (fill the frame, minimise glare) or connect
   an HDMI→USB capture dongle. Grant camera + microphone permissions.
6. Pick a scenario and tap **Run**.

---

## Building & running

### Android app (Android Studio)

Open `smart-display-eval/` in Android Studio (Giraffe+), let it sync, and run the
`app` configuration on a phone/tablet (min SDK 24). Requires the Android SDK.

```bash
./gradlew :app:assembleDebug   # requires Android SDK (ANDROID_HOME / local.properties)
```

### Core engine — build, test, demo (no Android SDK needed)

```bash
# Run the analysis verification suite (synthetic frames, 28 checks):
./gradlew :core:test

# Run the headless end-to-end demo on a simulated Fire TV session:
./gradlew :core:run              # human-readable report
./gradlew :core:run --args="--json"   # machine-gradable JSON
```

The Gradle build uses the Foojay toolchain resolver to auto‑provision JDK 17, so
it works regardless of the JDK on your machine.

---

## Extending

- **New scenarios**: add to `FireTvScenarios` (or build `Scenario`s
  programmatically from an agent). Key codes live in `RemoteKey`.
- **New detectors**: implement a per‑frame analyzer returning `ArtifactEvent`s
  and register it in `FrameAnalysisPipeline`. Add a `Verification` check.
- **Other devices**: `RemoteController` and the `TargetDevice`/`RemoteKey`
  mapping are device‑agnostic; Android TV works today, others need a controller
  and (if different) a key map.
- **Recorded input**: implement `FrameSource` over a recorded capture to
  re‑grade offline.

---

## Limitations & notes

- Camera observation is subject to optics/lighting; an HDMI capture path gives
  the cleanest artifact detection. Thresholds in each analyzer's `Config` are
  tunable per rig.
- The analyzers use robust, explainable heuristics rather than an ML model —
  deterministic and dependency‑free, which suits an eval harness. They can be
  swapped for learned detectors behind the same interfaces.
- The `app` module requires the Android SDK to build; the `core` module is fully
  self‑contained and is what the automated tests exercise.
