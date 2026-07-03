package com.smartdisplayeval.core.runner

import com.smartdisplayeval.core.analysis.ArtifactEvent
import com.smartdisplayeval.core.pipeline.FrameResult
import com.smartdisplayeval.core.report.StepResult
import com.smartdisplayeval.core.scenario.Step

/**
 * Optional live callbacks emitted while a [ScenarioRunner] executes, so a UI can
 * show progress in near real time (current scene, artifacts as they occur)
 * rather than only the final report. All callbacks are invoked on the runner's
 * thread; implementations should hand off to the UI thread as needed and return
 * quickly.
 */
interface RunnerListener {
    fun onStepStart(step: Step) {}
    fun onFrame(result: FrameResult) {}
    fun onArtifact(artifact: ArtifactEvent) {}
    fun onStepComplete(result: StepResult) {}
}
