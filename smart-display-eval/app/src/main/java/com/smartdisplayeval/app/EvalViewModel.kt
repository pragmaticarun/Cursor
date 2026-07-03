package com.smartdisplayeval.app

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.smartdisplayeval.app.control.AdbRemoteController
import com.smartdisplayeval.core.analysis.ArtifactEvent
import com.smartdisplayeval.core.analysis.SceneType
import com.smartdisplayeval.core.control.FrameSource
import com.smartdisplayeval.core.pipeline.FrameAnalysisPipeline
import com.smartdisplayeval.core.pipeline.FrameResult
import com.smartdisplayeval.core.report.EvalReport
import com.smartdisplayeval.core.report.StepResult
import com.smartdisplayeval.core.runner.RunnerListener
import com.smartdisplayeval.core.runner.ScenarioRunner
import com.smartdisplayeval.core.scenario.Scenario
import com.smartdisplayeval.core.scenario.Step
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

enum class ConnectionState { Disconnected, Connecting, Connected, Error }

/** Immutable UI state for the eval screen. */
data class EvalUiState(
    val host: String = "192.168.1.100",
    val connection: ConnectionState = ConnectionState.Disconnected,
    val running: Boolean = false,
    val currentStep: String? = null,
    val currentScene: SceneType = SceneType.UNKNOWN,
    val liveArtifacts: List<ArtifactEvent> = emptyList(),
    val report: EvalReport? = null,
    val message: String? = null,
)

/**
 * Coordinates connecting to the Fire TV and running scenarios. The heavy work
 * (ADB I/O and the blocking analysis loop) runs on background dispatchers; live
 * progress from the [ScenarioRunner] is marshalled back into [uiState].
 */
class EvalViewModel : ViewModel() {

    private val _uiState = MutableStateFlow(EvalUiState())
    val uiState: StateFlow<EvalUiState> = _uiState.asStateFlow()

    private var controller: AdbRemoteController? = null

    fun setHost(host: String) = _uiState.update { it.copy(host = host) }

    fun connect() {
        val host = _uiState.value.host.trim()
        if (host.isEmpty()) {
            _uiState.update { it.copy(connection = ConnectionState.Error, message = "Enter the Fire TV IP address") }
            return
        }
        _uiState.update { it.copy(connection = ConnectionState.Connecting, message = null) }
        viewModelScope.launch {
            val result = withContext(Dispatchers.IO) {
                runCatching { AdbRemoteController.connect(host) }
            }
            result.onSuccess { c ->
                controller = c
                _uiState.update { it.copy(connection = ConnectionState.Connected, message = "Connected to $host") }
            }.onFailure { e ->
                _uiState.update {
                    it.copy(
                        connection = ConnectionState.Error,
                        message = "Connect failed: ${e.message}. Enable ADB debugging on the Fire TV and accept the prompt.",
                    )
                }
            }
        }
    }

    /**
     * Runs [scenario], observing the display through [frameSource]. The frame
     * source must already be receiving frames (camera bound) before this runs.
     */
    fun run(scenario: Scenario, frameSource: FrameSource) {
        val ctrl = controller
        if (ctrl == null) {
            _uiState.update { it.copy(message = "Connect to the Fire TV first") }
            return
        }
        if (_uiState.value.running) return

        _uiState.update {
            it.copy(running = true, report = null, liveArtifacts = emptyList(), currentStep = null, message = null)
        }

        val listener = object : RunnerListener {
            override fun onStepStart(step: Step) {
                _uiState.update { it.copy(currentStep = step.name) }
            }

            override fun onFrame(result: FrameResult) {
                _uiState.update { it.copy(currentScene = result.scene.type) }
            }

            override fun onArtifact(artifact: ArtifactEvent) {
                _uiState.update { it.copy(liveArtifacts = it.liveArtifacts + artifact) }
            }

            override fun onStepComplete(result: StepResult) {}
        }

        viewModelScope.launch {
            val report = withContext(Dispatchers.IO) {
                ScenarioRunner(
                    controller = ctrl,
                    source = frameSource,
                    pipeline = FrameAnalysisPipeline(),
                    listener = listener,
                    keepResourcesOpen = true, // reuse the live ADB connection + camera across runs
                ).run(scenario)
            }
            _uiState.update {
                it.copy(running = false, report = report, currentStep = null, message = "Run complete")
            }
        }
    }

    override fun onCleared() {
        controller?.close()
        super.onCleared()
    }
}
