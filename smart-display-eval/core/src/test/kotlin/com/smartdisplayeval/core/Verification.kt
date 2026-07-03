package com.smartdisplayeval.core

import com.smartdisplayeval.core.analysis.ArtifactType
import com.smartdisplayeval.core.analysis.AudioLossDetector
import com.smartdisplayeval.core.analysis.AvSyncAnalyzer
import com.smartdisplayeval.core.analysis.BlackFrameDetector
import com.smartdisplayeval.core.analysis.FluidityAnalyzer
import com.smartdisplayeval.core.analysis.MacroblockDetector
import com.smartdisplayeval.core.analysis.SceneClassifier
import com.smartdisplayeval.core.analysis.SceneType
import com.smartdisplayeval.core.analysis.SharpnessAnalyzer
import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.pipeline.FrameAnalysisPipeline
import com.smartdisplayeval.core.runner.ScenarioRunner
import com.smartdisplayeval.core.scenario.Expectation
import com.smartdisplayeval.core.scenario.RemoteKey
import com.smartdisplayeval.core.scenario.Scenario
import com.smartdisplayeval.core.scenario.Step
import com.smartdisplayeval.core.scenario.StepAction
import com.smartdisplayeval.core.sim.RecordingRemoteController
import com.smartdisplayeval.core.sim.ScriptedFrameSource
import com.smartdisplayeval.core.sim.SyntheticFrames
import kotlin.math.abs

/**
 * Dependency-free verification suite for the analysis engine. Each `test*`
 * method exercises one analyzer against synthetic frames with known properties
 * and asserts the expected behaviour. [runAll] returns the number of failures
 * so it can be driven both from a standalone `main` and from a JUnit test.
 */
object Verification {

    private var failures = 0
    private var checks = 0

    fun runAll(): Int {
        failures = 0
        checks = 0
        testSceneStatic()
        testSceneUiScroll()
        testSceneVideo()
        testMacroblocking()
        testSharpnessDrop()
        testAudioLoss()
        testBlackFrame()
        testFluidityStall()
        testFluiditySmooth()
        testAvSyncOffset()
        testEndToEndScenario()
        testJsonReport()
        println("--------------------------------------------------")
        println("checks: $checks   failures: $failures")
        return failures
    }

    // ---- assertions -------------------------------------------------------

    private fun check(cond: Boolean, msg: String) {
        checks++
        if (cond) {
            println("  PASS  $msg")
        } else {
            failures++
            println("  FAIL  $msg")
        }
    }

    private fun section(name: String) = println("[$name]")

    // ---- scene classification --------------------------------------------

    private fun testSceneStatic() {
        section("scene: static UI")
        val c = SceneClassifier()
        c.classify(SyntheticFrames.staticUi(0, 0))
        val r = c.classify(SyntheticFrames.staticUi(1, 33))
        check(r.type == SceneType.STATIC, "identical UI frames classified STATIC (got ${r.type})")
    }

    private fun testSceneUiScroll() {
        section("scene: UI scrolling")
        val c = SceneClassifier()
        var last = SceneType.UNKNOWN
        var uiCount = 0
        for (i in 0 until 8) {
            val r = c.classify(SyntheticFrames.uiScroll(i, i * 33L, scrollY = i * 4))
            last = r.type
            if (r.type == SceneType.UI_MOTION) uiCount++
        }
        check(uiCount >= 5, "scrolling UI classified UI_MOTION for most frames (got $uiCount/7, last=$last)")
    }

    private fun testSceneVideo() {
        section("scene: video playback")
        val c = SceneClassifier()
        var videoCount = 0
        for (i in 0 until 10) {
            val r = c.classify(SyntheticFrames.video(i, i * 33L, phase = i * 0.35))
            if (r.type == SceneType.VIDEO) videoCount++
        }
        check(videoCount >= 6, "natural video classified VIDEO for most frames (got $videoCount/9)")
    }

    // ---- macroblocking ----------------------------------------------------

    private fun testMacroblocking() {
        section("artifact: macroblocking")
        val d = MacroblockDetector()
        // Establish a clean baseline on smooth video.
        var cleanEvents = 0
        for (i in 0 until 6) {
            if (d.analyze(SyntheticFrames.video(i, i * 33L, phase = i * 0.3)) != null) cleanEvents++
        }
        check(cleanEvents == 0, "no macroblocking flagged on clean video (got $cleanEvents)")

        var blockEvents = 0
        for (i in 6 until 12) {
            if (d.analyze(SyntheticFrames.macroblockedVideo(i, i * 33L, phase = i * 0.3)) != null) blockEvents++
        }
        check(blockEvents >= 1, "macroblocking flagged on blocky video (got $blockEvents)")
    }

    // ---- sharpness / low resolution --------------------------------------

    private fun testSharpnessDrop() {
        section("artifact: low resolution / softness")
        val d = SharpnessAnalyzer()
        for (i in 0 until 6) d.analyze(SyntheticFrames.video(i, i * 33L, phase = i * 0.3))
        var softEvents = 0
        for (i in 6 until 14) {
            val soft = SyntheticFrames.blur(SyntheticFrames.video(i, i * 33L, phase = i * 0.3), radius = 4)
            if (d.analyze(soft) != null) softEvents++
        }
        check(softEvents >= 1, "low-resolution flagged after sustained blur (got $softEvents)")
    }

    // ---- audio loss -------------------------------------------------------

    private fun testAudioLoss() {
        section("artifact: audio loss")
        val d = AudioLossDetector()
        val classifier = SceneClassifier()
        var event = false
        var eventTs = -1L
        // Play video with audio, then cut audio while picture keeps moving.
        for (i in 0 until 30) {
            val hasAudio = i < 10
            val f = SyntheticFrames.video(i, i * 33L, phase = i * 0.3, audioRms = if (hasAudio) 0.4 else 0.0)
            val motion = classifier.classify(f).motion
            val e = d.analyze(f, motion)
            if (e != null) { event = true; eventTs = e.timestampMs }
        }
        check(event, "audio dropout detected while picture active")
        check(eventTs in 300..500, "dropout timestamped near the silence onset (got ${eventTs}ms)")
    }

    // ---- black frame ------------------------------------------------------

    private fun testBlackFrame() {
        section("artifact: black frame")
        val d = BlackFrameDetector()
        check(d.analyze(SyntheticFrames.video(0, 0, 0.0)) == null, "normal frame is not black")
        check(d.analyze(SyntheticFrames.black(1, 33)) != null, "black frame detected")
    }

    // ---- fluidity ---------------------------------------------------------

    private fun testFluidityStall() {
        section("fluidity: freeze / stall during motion")
        val d = FluidityAnalyzer()
        var t = 0L
        var idx = 0
        var stall = false
        // Real motion for a while.
        for (i in 0 until 15) {
            if (d.analyze(t, motion = 0.05, SceneType.VIDEO, idx++) != null) stall = true
            t += 33
        }
        // Freeze: motion collapses to ~0 for > stallMs.
        for (i in 0 until 12) {
            if (d.analyze(t, motion = 0.0, SceneType.VIDEO, idx++) != null) stall = true
            t += 33
        }
        check(stall, "freeze during motion flagged as FRAME_STALL")
        val report = d.summarize()
        check(report.stallCount >= 1, "stall counted in fluidity report (got ${report.stallCount})")
        check(report.longestStallMs >= 180, "longest stall recorded (got ${report.longestStallMs}ms)")
    }

    private fun testFluiditySmooth() {
        section("fluidity: smooth motion scores high")
        val d = FluidityAnalyzer()
        var t = 0L
        for (i in 0 until 40) {
            d.analyze(t, motion = 0.05, SceneType.VIDEO, i)
            t += 16 // ~60fps steady
        }
        val report = d.summarize()
        check(report.stallCount == 0, "no stalls on steady motion")
        check(report.smoothness >= 85.0, "smooth motion scores high (got %.1f)".format(report.smoothness))
    }

    // ---- AV sync ----------------------------------------------------------

    private fun testAvSyncOffset() {
        section("av-sync: known audio delay recovered")
        val d = AvSyncAnalyzer()
        val bin = 33L
        val delayBins = 3
        val n = 240
        // Irregular, well-separated pulses (spacing >> search window) so the
        // cross-correlation has a single unambiguous peak at the true lag.
        val videoPulses = intArrayOf(15, 43, 74, 100, 131, 162, 190, 220)
        val audioPulses = videoPulses.map { it + delayBins }.toIntArray()
        for (i in 0 until n) {
            val t = i * bin
            val motion = if (i in videoPulses) 0.15 else 0.01
            val audioLead = if (i in audioPulses) 0.5 else 0.02
            d.add(t, audioRms = audioLead, motion = motion)
        }
        val r = d.summarize()
        check(r != null, "av-sync produced a result")
        if (r != null) {
            val expected = delayBins * bin
            check(abs(r.offsetMs - expected) <= bin, "recovered audio delay ~${expected}ms (got ${r.offsetMs}ms)")
            check(r.correlation >= 0.5, "cross-correlation is strong (got %.2f)".format(r.correlation))
        }
    }

    // ---- end to end runner ------------------------------------------------

    private fun buildFrames(): List<Frame> {
        val frames = ArrayList<Frame>()
        var idx = 0
        var t = 0L
        // Step 1 window: ~1s of UI scrolling.
        for (i in 0 until 30) {
            frames.add(SyntheticFrames.uiScroll(idx++, t, scrollY = i * 4)); t += 33
        }
        // Step 2 window: ~1.3s of video, with a macroblocked burst in the middle.
        for (i in 0 until 40) {
            val f = if (i in 15..22) SyntheticFrames.macroblockedVideo(idx, t, phase = i * 0.3)
            else SyntheticFrames.video(idx, t, phase = i * 0.3)
            frames.add(f); idx++; t += 33
        }
        return frames
    }

    private fun buildScenario() = Scenario(
        name = "Fire TV: browse then play",
        description = "Scroll a menu, then start playback",
        steps = listOf(
            Step(
                name = "Scroll menu",
                action = StepAction.PressKey(RemoteKey.DPAD_DOWN, repeat = 5),
                observeMs = 990,
                expectations = listOf(Expectation.SceneShouldBe(SceneType.UI_MOTION, minFraction = 0.5)),
            ),
            Step(
                name = "Play title",
                action = StepAction.PressKey(RemoteKey.DPAD_CENTER),
                observeMs = 1300,
                expectations = listOf(Expectation.SceneShouldBe(SceneType.VIDEO, minFraction = 0.5)),
            ),
        ),
    )

    private fun testEndToEndScenario() {
        section("runner: end to end scenario")
        val controller = RecordingRemoteController()
        val runner = ScenarioRunner(controller, ScriptedFrameSource(buildFrames()), FrameAnalysisPipeline())
        val report = runner.run(buildScenario())

        check(report.stepResults.size == 2, "both steps ran (got ${report.stepResults.size})")
        check(controller.commands.size == 2, "controller received both actions (got ${controller.commands.size})")
        check(report.stepResults[0].passed, "step 1 (UI scroll) met its expectation")
        check(report.stepResults[1].passed, "step 2 (video) met its expectation")
        val macro = report.artifacts.count { it.type == ArtifactType.MACROBLOCKING }
        check(macro >= 1, "macroblocking burst captured in report (got $macro)")
        check(report.score() in 0.0..100.0, "score within range (got %.1f)".format(report.score()))
    }

    private fun testJsonReport() {
        section("report: JSON serialisation")
        val controller = RecordingRemoteController()
        val runner = ScenarioRunner(controller, ScriptedFrameSource(buildFrames()), FrameAnalysisPipeline())
        val json = runner.run(buildScenario()).toJson()
        check(json.startsWith("{") && json.endsWith("}"), "JSON is a single object")
        check(json.contains("\"scenario\""), "JSON contains scenario field")
        check(json.contains("\"artifacts\""), "JSON contains artifacts field")
        check(json.contains("\"score\""), "JSON contains score field")
    }
}
