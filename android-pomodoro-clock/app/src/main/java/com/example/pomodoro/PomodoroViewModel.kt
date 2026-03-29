package com.example.pomodoro

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch

class PomodoroViewModel : ViewModel() {
    private val engine = PomodoroTimerEngine()
    private val _state = MutableStateFlow(PomodoroState())
    val state: StateFlow<PomodoroState> = _state.asStateFlow()

    private var tickerJob: Job? = null

    fun start() {
        if (_state.value.isRunning) return
        _state.update { engine.start(it) }
        ensureTicker()
    }

    fun pause() {
        _state.update { engine.pause(it) }
    }

    fun resetCurrentPhase() {
        _state.update { engine.resetCurrentPhase(it) }
    }

    fun skipToNextPhase() {
        _state.update { engine.skipToNextPhase(it) }
    }

    fun updateWorkDuration(minutes: Int) {
        _state.update { engine.updateWorkDuration(it, minutes) }
    }

    fun updateShortBreakDuration(minutes: Int) {
        _state.update { engine.updateShortBreakDuration(it, minutes) }
    }

    fun updateLongBreakDuration(minutes: Int) {
        _state.update { engine.updateLongBreakDuration(it, minutes) }
    }

    fun setAutoStart(enabled: Boolean) {
        _state.update { engine.setAutoStart(it, enabled) }
    }

    private fun ensureTicker() {
        if (tickerJob?.isActive == true) return
        tickerJob = viewModelScope.launch {
            while (isActive) {
                delay(1_000L)
                _state.update { current ->
                    engine.tick(current)
                }
            }
        }
    }

    override fun onCleared() {
        tickerJob?.cancel()
        super.onCleared()
    }
}
