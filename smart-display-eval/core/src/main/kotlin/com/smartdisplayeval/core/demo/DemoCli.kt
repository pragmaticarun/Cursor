package com.smartdisplayeval.core.demo

import com.smartdisplayeval.core.analysis.SceneType
import com.smartdisplayeval.core.control.FrameSource
import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.pipeline.FrameAnalysisPipeline
import com.smartdisplayeval.core.report.EvalReport
import com.smartdisplayeval.core.runner.ScenarioRunner
import com.smartdisplayeval.core.scenario.Expectation
import com.smartdisplayeval.core.scenario.RemoteKey
import com.smartdisplayeval.core.scenario.Scenario
import com.smartdisplayeval.core.scenario.Step
import com.smartdisplayeval.core.scenario.StepAction
import com.smartdisplayeval.core.scenario.TargetDevice
import com.smartdisplayeval.core.sim.RecordingRemoteController
import com.smartdisplayeval.core.sim.ScriptedFrameSource
import com.smartdisplayeval.core.sim.SyntheticFrames

/**
 * A headless, hardware-free demonstration of the whole eval pipeline.
 *
 * It builds a Fire-TV-style scenario (home -> browse -> open app -> play), and
 * a synthetic "capture" of that session that deliberately contains a
 * macroblocking burst, an audio dropout and a playback freeze. Running the
 * scenario through [ScenarioRunner] produces the same [EvalReport] the real
 * on-device app would, which is printed both as a human-readable summary and as
 * the machine-gradable JSON an agentic workflow would consume.
 *
 * Run with:  java -cp core.jar com.smartdisplayeval.core.demo.DemoCliKt
 */
fun main(args: Array<String>) {
    val emitJson = args.contains("--json")

    val scenario = Scenario(
        name = "Fire TV Stick: browse Prime Video and play a title",
        description = "Wake, scroll the home row, open an app, start playback, verify picture/sound.",
        target = TargetDevice.FIRE_TV_STICK,
        steps = listOf(
            Step(
                name = "Wake and go Home",
                action = StepAction.PressKey(RemoteKey.HOME),
                observeMs = 660,
                expectations = listOf(Expectation.SceneShouldBe(SceneType.STATIC, minFraction = 0.5)),
            ),
            Step(
                name = "Scroll the home carousel",
                action = StepAction.PressKey(RemoteKey.DPAD_RIGHT, repeat = 6),
                observeMs = 990,
                expectations = listOf(
                    Expectation.SceneShouldBe(SceneType.UI_MOTION, minFraction = 0.5),
                    Expectation.MinFluidity(min = 60.0),
                ),
            ),
            Step(
                name = "Open title and play",
                action = StepAction.PressKey(RemoteKey.DPAD_CENTER),
                observeMs = 2_310,
                expectations = listOf(
                    Expectation.SceneShouldBe(SceneType.VIDEO, minFraction = 0.5),
                    Expectation.NoArtifacts(
                        types = setOf(
                            com.smartdisplayeval.core.analysis.ArtifactType.MACROBLOCKING,
                            com.smartdisplayeval.core.analysis.ArtifactType.AUDIO_LOSS,
                            com.smartdisplayeval.core.analysis.ArtifactType.FRAME_STALL,
                        ),
                        maxSeverity = 0.1,
                    ),
                ),
            ),
        ),
    )

    val report = ScenarioRunner(
        controller = RecordingRemoteController(),
        source = syntheticSession(),
        pipeline = FrameAnalysisPipeline(),
    ).run(scenario)

    if (emitJson) {
        println(report.toJson())
        return
    }
    printReport(report)
}

/** Builds a synthetic capture of the scenario with a few injected defects. */
private fun syntheticSession(): FrameSource {
    val frames = ArrayList<Frame>()
    var idx = 0
    var t = 0L
    val dt = 33L // ~30 fps capture

    fun add(f: Frame) { frames.add(f); idx++; t += dt }

    // Step 1: Home screen at rest.
    repeat(20) { add(SyntheticFrames.staticUi(idx, t, jitter = 1)) }

    // Step 2: carousel scrolling.
    repeat(30) { i -> add(SyntheticFrames.uiScroll(idx, t, scrollY = i * 5)) }

    // Step 3: video playback with injected artifacts.
    repeat(75) { i ->
        val f = when {
            i in 20..27 -> SyntheticFrames.macroblockedVideo(idx, t, phase = i * 0.3) // macroblocking burst
            i in 40..55 -> SyntheticFrames.video(idx, t, phase = i * 0.3, audioRms = 0.0) // ~528ms audio dropout
            i in 60..67 -> SyntheticFrames.video(20, t, phase = 6.0) // repeated content -> freeze/stall
            else -> SyntheticFrames.video(idx, t, phase = i * 0.3, audioRms = 0.4)
        }
        add(f)
    }
    return ScriptedFrameSource(frames)
}

private fun printReport(report: EvalReport) {
    val line = "=".repeat(70)
    println(line)
    println("SMART DISPLAY EVAL REPORT")
    println(line)
    println("Scenario : ${report.scenarioName}")
    println("Target   : ${report.target}")
    println("Duration : ${report.durationMs} ms")
    println("Passed   : ${report.passed}")
    println("Score    : %.1f / 100".format(report.score()))
    println()
    println("Fluidity : %.1f fps observed, jitter %.1f ms, smoothness %.1f, stalls %d (longest %d ms)".format(
        report.fluidity.measuredFps,
        report.fluidity.captureJitterMs,
        report.fluidity.smoothness,
        report.fluidity.stallCount,
        report.fluidity.longestStallMs,
    ))
    report.avSync?.let {
        println("AV sync  : offset %d ms (corr %.2f, confidence %.2f)".format(it.offsetMs, it.correlation, it.confidence))
    }
    println()
    println("STEPS")
    for (s in report.stepResults) {
        val mark = if (s.passed) "PASS" else "FAIL"
        println("  [$mark] ${s.name}  (${s.actionDescription})  ${s.startMs}-${s.endMs}ms")
        for (e in s.expectationResults) {
            val em = if (e.passed) "ok  " else "FAIL"
            println("        - [$em] ${e.description}  -> ${e.detail}")
        }
    }
    println()
    println("ARTIFACT TIMELINE (${report.artifacts.size})")
    if (report.artifacts.isEmpty()) {
        println("  (none)")
    } else {
        for (a in report.artifacts.sortedBy { it.timestampMs }) {
            println("  @%6dms  %-13s sev=%.2f conf=%.2f  %s".format(
                a.timestampMs, a.type.name, a.severity, a.confidence, a.detail,
            ))
        }
    }
    println(line)
    println("Tip: re-run with --json to emit the machine-gradable report.")
}
