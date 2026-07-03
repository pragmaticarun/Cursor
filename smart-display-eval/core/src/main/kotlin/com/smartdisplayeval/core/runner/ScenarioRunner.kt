package com.smartdisplayeval.core.runner

import com.smartdisplayeval.core.analysis.ArtifactEvent
import com.smartdisplayeval.core.analysis.AvSyncAnalyzer
import com.smartdisplayeval.core.analysis.FluidityAnalyzer
import com.smartdisplayeval.core.analysis.SceneType
import com.smartdisplayeval.core.control.FrameSource
import com.smartdisplayeval.core.control.RemoteController
import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.pipeline.FrameAnalysisPipeline
import com.smartdisplayeval.core.report.EvalReport
import com.smartdisplayeval.core.report.ExpectationResult
import com.smartdisplayeval.core.report.StepResult
import com.smartdisplayeval.core.scenario.Expectation
import com.smartdisplayeval.core.scenario.Scenario
import com.smartdisplayeval.core.scenario.Step
import com.smartdisplayeval.core.scenario.StepAction
import kotlin.math.abs

/**
 * Drives a [Scenario] against a device: for each step it performs the action
 * through the [RemoteController], observes the display via the [FrameSource]
 * for the step's window, runs the analysis [FrameAnalysisPipeline] in near real
 * time, then checks the step's expectations. Produces an [EvalReport].
 *
 * Time is measured from frame timestamps so the same runner works for a live
 * capture and for a deterministic recorded/synthetic source (used in tests and
 * the demo). The [RemoteController] is invoked but its wall-clock delays do not
 * drive the observation window; the frame stream does.
 */
class ScenarioRunner(
    private val controller: RemoteController,
    private val source: FrameSource,
    private val pipeline: FrameAnalysisPipeline = FrameAnalysisPipeline(),
) {
    /** One-frame lookahead so we can decide when a step's window is complete. */
    private var pending: Frame? = null
    private var lastTimestampMs: Long = 0

    fun run(scenario: Scenario): EvalReport {
        pipeline.reset()
        pending = source.next()
        val startMs = pending?.timestampMs ?: 0
        lastTimestampMs = startMs

        val stepResults = ArrayList<StepResult>()
        for (step in scenario.steps) {
            stepResults.add(runStep(step))
            if (pending == null) break // frame source exhausted
        }

        val summary = pipeline.finish(avSyncMaxMs = null, lastTimestampMs = lastTimestampMs)
        controller.close()
        source.close()

        return EvalReport(
            scenarioName = scenario.name,
            target = scenario.target.name,
            startedAtMs = startMs,
            durationMs = lastTimestampMs - startMs,
            stepResults = stepResults,
            artifacts = summary.artifacts,
            fluidity = summary.fluidity,
            avSync = summary.avSync,
        )
    }

    private fun runStep(step: Step): StepResult {
        dispatch(step.action)

        val windowStart = lastTimestampMs
        val collector = StepCollector()

        // Consume frames until the observation window elapses (by frame time).
        while (true) {
            val frame = pending ?: break
            if (frame.timestampMs - windowStart >= step.observeMs) break
            pending = source.next()
            lastTimestampMs = frame.timestampMs

            val result = pipeline.onFrame(frame)
            collector.accept(frame, result.scene.type, result.scene.motion, result.artifacts)
        }

        val expectationResults = step.expectations.map { evaluate(it, collector) }
        return StepResult(
            name = step.name,
            actionDescription = describe(step.action),
            startMs = windowStart,
            endMs = lastTimestampMs,
            expectationResults = expectationResults,
            artifacts = collector.artifacts,
        )
    }

    private fun dispatch(action: StepAction) {
        when (action) {
            is StepAction.PressKey -> controller.press(action.key, action.repeat, action.intervalMs)
            is StepAction.LaunchApp -> controller.launchApp(action.packageName, action.activity)
            is StepAction.InputText -> controller.inputText(action.text)
            StepAction.Idle -> {}
        }
    }

    private fun describe(action: StepAction): String = when (action) {
        is StepAction.PressKey -> "press ${action.key.label}" + if (action.repeat > 1) " x${action.repeat}" else ""
        is StepAction.LaunchApp -> "launch ${action.packageName}"
        is StepAction.InputText -> "type \"${action.text}\""
        StepAction.Idle -> "idle / observe"
    }

    private fun evaluate(expectation: Expectation, c: StepCollector): ExpectationResult = when (expectation) {
        is Expectation.SceneShouldBe -> {
            val frac = c.sceneFraction(expectation.type)
            ExpectationResult(
                description = "scene is ${expectation.type} for >= ${(expectation.minFraction * 100).toInt()}% of window",
                passed = frac >= expectation.minFraction,
                detail = "observed ${(frac * 100).toInt()}% ${expectation.type}",
            )
        }
        is Expectation.MinFluidity -> {
            val s = c.fluidity.summarize().smoothness
            ExpectationResult(
                description = "fluidity smoothness >= ${expectation.min}",
                passed = s >= expectation.min,
                detail = "smoothness=%.1f".format(s),
            )
        }
        is Expectation.NoArtifacts -> {
            val offending = c.artifacts.filter {
                it.type in expectation.types && it.severity > expectation.maxSeverity
            }
            ExpectationResult(
                description = "no ${expectation.types.joinToString("/") { it.name }} above severity ${expectation.maxSeverity}",
                passed = offending.isEmpty(),
                detail = if (offending.isEmpty()) "none detected"
                else offending.joinToString("; ") { "${it.type}@${it.timestampMs}ms sev=%.2f".format(it.severity) },
            )
        }
        is Expectation.MaxAvSyncMs -> {
            val r = c.avSync.summarize()
            val offset = r?.offsetMs ?: 0
            val ok = r == null || r.confidence < 0.3 || abs(offset) <= expectation.maxAbsMs
            ExpectationResult(
                description = "|AV sync offset| <= ${expectation.maxAbsMs}ms",
                passed = ok,
                detail = if (r == null) "insufficient audio data"
                else "offset=${offset}ms corr=%.2f".format(r.correlation),
            )
        }
    }

    /** Per-step accumulation used purely to evaluate that step's expectations. */
    private class StepCollector {
        val fluidity = FluidityAnalyzer()
        val avSync = AvSyncAnalyzer()
        val artifacts = ArrayList<ArtifactEvent>()
        private val sceneCounts = HashMap<SceneType, Int>()
        private var frames = 0

        fun accept(frame: Frame, scene: SceneType, motion: Double, found: List<ArtifactEvent>) {
            frames++
            sceneCounts[scene] = (sceneCounts[scene] ?: 0) + 1
            artifacts.addAll(found)
            fluidity.analyze(frame.timestampMs, motion, scene, frame.index)
            frame.audio?.let { avSync.add(frame.timestampMs, it.rms, motion) }
        }

        fun sceneFraction(type: SceneType): Double =
            if (frames == 0) 0.0 else (sceneCounts[type] ?: 0).toDouble() / frames
    }
}
