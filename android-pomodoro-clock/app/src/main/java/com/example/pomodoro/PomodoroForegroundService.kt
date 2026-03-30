package com.example.pomodoro

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.app.Service
import android.content.Intent
import android.os.Binder
import android.os.Build
import android.os.IBinder
import androidx.core.app.NotificationCompat
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancel
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch

class PomodoroForegroundService : Service() {

    private val engine = PomodoroTimerEngine()
    private val serviceScope = CoroutineScope(SupervisorJob() + Dispatchers.Default)
    private val binder = LocalBinder()
    private val _state = MutableStateFlow(PomodoroState())
    private var tickerJob: Job? = null

    inner class LocalBinder : Binder() {
        fun service(): PomodoroForegroundService = this@PomodoroForegroundService
    }

    override fun onCreate() {
        super.onCreate()
        createNotificationChannel()
    }

    override fun onBind(intent: Intent?): IBinder = binder

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        val action = intent?.action ?: PomodoroServiceContract.ACTION_NOOP
        val providedState = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            intent?.getParcelableExtra(PomodoroServiceContract.EXTRA_STATE, PomodoroState::class.java)
        } else {
            @Suppress("DEPRECATION")
            intent?.getParcelableExtra(PomodoroServiceContract.EXTRA_STATE)
        }
        if (providedState != null) {
            _state.value = providedState
        }

        when (action) {
            PomodoroServiceContract.ACTION_START -> applyState { engine.start(it) }
            PomodoroServiceContract.ACTION_PAUSE -> applyState { engine.pause(it) }
            PomodoroServiceContract.ACTION_RESET -> applyState { engine.resetCurrentPhase(it) }
            PomodoroServiceContract.ACTION_SKIP -> applyState { engine.skipToNextPhase(it) }
            PomodoroServiceContract.ACTION_SET_WORK_DURATION -> {
                val minutes = intent?.getIntExtra(PomodoroServiceContract.EXTRA_MINUTES, -1) ?: -1
                if (minutes >= PomodoroTimerEngine.MIN_DURATION_MINUTES) {
                    applyState { engine.updateWorkDuration(it, minutes) }
                }
            }
            PomodoroServiceContract.ACTION_SET_SHORT_BREAK_DURATION -> {
                val minutes = intent?.getIntExtra(PomodoroServiceContract.EXTRA_MINUTES, -1) ?: -1
                if (minutes >= PomodoroTimerEngine.MIN_DURATION_MINUTES) {
                    applyState { engine.updateShortBreakDuration(it, minutes) }
                }
            }
            PomodoroServiceContract.ACTION_SET_LONG_BREAK_DURATION -> {
                val minutes = intent?.getIntExtra(PomodoroServiceContract.EXTRA_MINUTES, -1) ?: -1
                if (minutes >= PomodoroTimerEngine.MIN_DURATION_MINUTES) {
                    applyState { engine.updateLongBreakDuration(it, minutes) }
                }
            }
            PomodoroServiceContract.ACTION_SET_AUTO_START -> {
                val enabled = intent?.getBooleanExtra(PomodoroServiceContract.EXTRA_ENABLED, true) ?: true
                applyState { engine.setAutoStart(it, enabled) }
            }
            PomodoroServiceContract.ACTION_SYNC_STATE -> {
                broadcastState()
                updateNotification()
            }
            PomodoroServiceContract.ACTION_STOP_SERVICE -> {
                applyState { engine.pause(it) }
                stopForeground(STOP_FOREGROUND_REMOVE)
                stopSelf()
                return START_NOT_STICKY
            }
            PomodoroServiceContract.ACTION_NOOP -> Unit
        }

        startForeground(
            PomodoroServiceContract.NOTIFICATION_ID,
            buildNotification(_state.value)
        )
        return START_STICKY
    }

    override fun onDestroy() {
        broadcastState()
        tickerJob?.cancel()
        serviceScope.cancel()
        super.onDestroy()
    }

    fun state(): StateFlow<PomodoroState> = _state.asStateFlow()

    fun startTimer() = applyState { engine.start(it) }

    fun pauseTimer() = applyState { engine.pause(it) }

    fun resetCurrentPhase() = applyState { engine.resetCurrentPhase(it) }

    fun skipToNextPhase() = applyState { engine.skipToNextPhase(it) }

    fun updateWorkDuration(minutes: Int) = applyState { engine.updateWorkDuration(it, minutes) }

    fun updateShortBreakDuration(minutes: Int) = applyState { engine.updateShortBreakDuration(it, minutes) }

    fun updateLongBreakDuration(minutes: Int) = applyState { engine.updateLongBreakDuration(it, minutes) }

    fun setAutoStart(enabled: Boolean) = applyState { engine.setAutoStart(it, enabled) }

    private fun applyState(transform: (PomodoroState) -> PomodoroState) {
        _state.value = transform(_state.value)
        syncTickerForCurrentState()
        broadcastState()
        updateNotification()
    }

    private fun syncTickerForCurrentState() {
        if (_state.value.isRunning) {
            if (tickerJob?.isActive == true) return
            tickerJob = serviceScope.launch {
                while (isActive) {
                    delay(1_000L)
                    val next = engine.tick(_state.value)
                    if (next != _state.value) {
                        _state.value = next
                        broadcastState()
                        updateNotification()
                    }
                }
            }
        } else {
            tickerJob?.cancel()
            tickerJob = null
        }
    }

    private fun updateNotification() {
        val manager = getSystemService(NotificationManager::class.java)
        manager.notify(
            PomodoroServiceContract.NOTIFICATION_ID,
            buildNotification(_state.value)
        )
    }

    private fun buildNotification(state: PomodoroState): Notification {
        val contentIntent = PendingIntent.getActivity(
            this,
            10,
            Intent(this, MainActivity::class.java).apply {
                flags = Intent.FLAG_ACTIVITY_SINGLE_TOP
            },
            PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_IMMUTABLE
        )

        val toggleAction = if (state.isRunning) {
            NotificationCompat.Action(
                0,
                getString(R.string.pause),
                servicePendingIntent(PomodoroServiceContract.ACTION_PAUSE, 1)
            )
        } else {
            NotificationCompat.Action(
                0,
                getString(R.string.start),
                servicePendingIntent(PomodoroServiceContract.ACTION_START, 1)
            )
        }

        return NotificationCompat.Builder(this, PomodoroServiceContract.NOTIFICATION_CHANNEL_ID)
            .setSmallIcon(android.R.drawable.ic_popup_reminder)
            .setContentTitle(state.phase.notificationLabel(this))
            .setContentText(
                getString(
                    R.string.notification_content_template,
                    state.phase.notificationLabel(this),
                    state.formattedRemainingTime()
                )
            )
            .setContentIntent(contentIntent)
            .setOnlyAlertOnce(true)
            .setOngoing(true)
            .setPriority(NotificationCompat.PRIORITY_LOW)
            .addAction(toggleAction)
            .addAction(
                NotificationCompat.Action(
                    0,
                    getString(R.string.reset),
                    servicePendingIntent(PomodoroServiceContract.ACTION_RESET, 2)
                )
            )
            .addAction(
                NotificationCompat.Action(
                    0,
                    getString(R.string.skip),
                    servicePendingIntent(PomodoroServiceContract.ACTION_SKIP, 3)
                )
            )
            .addAction(
                NotificationCompat.Action(
                    0,
                    getString(R.string.stop_service),
                    servicePendingIntent(PomodoroServiceContract.ACTION_STOP_SERVICE, 4)
                )
            )
            .build()
    }

    private fun servicePendingIntent(action: String, requestCode: Int): PendingIntent {
        val intent = Intent(this, PomodoroForegroundService::class.java).apply {
            this.action = action
        }
        return PendingIntent.getService(
            this,
            requestCode,
            intent,
            PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_IMMUTABLE
        )
    }

    private fun createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                PomodoroServiceContract.NOTIFICATION_CHANNEL_ID,
                PomodoroServiceContract.NOTIFICATION_CHANNEL_NAME,
                NotificationManager.IMPORTANCE_LOW
            ).apply {
                description = getString(R.string.notification_channel_description)
            }
            val manager = getSystemService(NotificationManager::class.java)
            manager.createNotificationChannel(channel)
        }
    }

    private fun broadcastState() {
        sendBroadcast(
            Intent(PomodoroServiceContract.ACTION_STATE_CHANGED).apply {
                setPackage(packageName)
                putExtra(PomodoroServiceContract.EXTRA_STATE, _state.value)
            }
        )
    }
}

private fun PomodoroPhase.notificationLabel(context: android.content.Context): String = when (this) {
    PomodoroPhase.WORK -> context.getString(R.string.focus_time)
    PomodoroPhase.SHORT_BREAK -> context.getString(R.string.short_break)
    PomodoroPhase.LONG_BREAK -> context.getString(R.string.long_break)
}
