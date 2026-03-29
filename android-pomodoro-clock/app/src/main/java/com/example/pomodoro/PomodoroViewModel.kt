package com.example.pomodoro

import android.app.Application
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Build
import androidx.core.content.ContextCompat
import androidx.lifecycle.AndroidViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

class PomodoroViewModel(application: Application) : AndroidViewModel(application) {
    private val app = application
    private val _state = MutableStateFlow(PomodoroState())
    val state: StateFlow<PomodoroState> = _state.asStateFlow()
    private val _isServiceConnected = MutableStateFlow(false)
    val isServiceConnected: StateFlow<Boolean> = _isServiceConnected.asStateFlow()

    private var isReceiverRegistered = false
    private val stateReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            if (intent?.action != PomodoroServiceContract.ACTION_STATE_CHANGED) return
            val updated = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                intent.getParcelableExtra(PomodoroServiceContract.EXTRA_STATE, PomodoroState::class.java)
            } else {
                @Suppress("DEPRECATION")
                intent.getParcelableExtra(PomodoroServiceContract.EXTRA_STATE)
            }
            if (updated != null) {
                _state.value = updated
            }
        }
    }

    fun start() = sendAction(PomodoroServiceContract.ACTION_START)

    fun pause() = sendAction(PomodoroServiceContract.ACTION_PAUSE)

    fun resetCurrentPhase() = sendAction(PomodoroServiceContract.ACTION_RESET)

    fun skipToNextPhase() = sendAction(PomodoroServiceContract.ACTION_SKIP)

    fun updateWorkDuration(minutes: Int) = sendAction(
        action = PomodoroServiceContract.ACTION_SET_WORK_DURATION,
        minutes = minutes
    )

    fun updateShortBreakDuration(minutes: Int) = sendAction(
        action = PomodoroServiceContract.ACTION_SET_SHORT_BREAK_DURATION,
        minutes = minutes
    )

    fun updateLongBreakDuration(minutes: Int) = sendAction(
        action = PomodoroServiceContract.ACTION_SET_LONG_BREAK_DURATION,
        minutes = minutes
    )

    fun setAutoStart(enabled: Boolean) = sendAction(
        action = PomodoroServiceContract.ACTION_SET_AUTO_START,
        enabled = enabled
    )

    fun registerStateReceiver() {
        if (isReceiverRegistered) return
        val filter = IntentFilter(PomodoroServiceContract.ACTION_STATE_CHANGED)
        ContextCompat.registerReceiver(
            app,
            stateReceiver,
            filter,
            ContextCompat.RECEIVER_NOT_EXPORTED
        )
        isReceiverRegistered = true
        _isServiceConnected.value = true
        sendAction(PomodoroServiceContract.ACTION_SYNC_STATE)
    }

    fun unregisterStateReceiver() {
        if (!isReceiverRegistered) return
        app.unregisterReceiver(stateReceiver)
        isReceiverRegistered = false
        _isServiceConnected.value = false
    }

    override fun onCleared() {
        unregisterStateReceiver()
        super.onCleared()
    }

    private fun sendAction(action: String, minutes: Int? = null, enabled: Boolean? = null) {
        val intent = Intent(app, PomodoroForegroundService::class.java).apply {
            this.action = action
            minutes?.let { putExtra(PomodoroServiceContract.EXTRA_MINUTES, it) }
            enabled?.let { putExtra(PomodoroServiceContract.EXTRA_ENABLED, it) }
        }
        ContextCompat.startForegroundService(app, intent)
    }
}
