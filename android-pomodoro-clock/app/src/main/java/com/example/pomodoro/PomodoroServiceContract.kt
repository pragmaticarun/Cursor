package com.example.pomodoro

object PomodoroServiceContract {
    const val ACTION_START = "com.example.pomodoro.action.START"
    const val ACTION_PAUSE = "com.example.pomodoro.action.PAUSE"
    const val ACTION_RESET = "com.example.pomodoro.action.RESET"
    const val ACTION_SKIP = "com.example.pomodoro.action.SKIP"
    const val ACTION_SYNC_STATE = "com.example.pomodoro.action.SYNC_STATE"
    const val ACTION_STOP_SERVICE = "com.example.pomodoro.action.STOP_SERVICE"
    const val ACTION_SET_WORK_DURATION = "com.example.pomodoro.action.SET_WORK_DURATION"
    const val ACTION_SET_SHORT_BREAK_DURATION = "com.example.pomodoro.action.SET_SHORT_BREAK_DURATION"
    const val ACTION_SET_LONG_BREAK_DURATION = "com.example.pomodoro.action.SET_LONG_BREAK_DURATION"
    const val ACTION_SET_AUTO_START = "com.example.pomodoro.action.SET_AUTO_START"
    const val ACTION_NOOP = "com.example.pomodoro.action.NOOP"
    const val ACTION_STATE_CHANGED = "com.example.pomodoro.action.STATE_CHANGED"

    const val EXTRA_MINUTES = "extra_minutes"
    const val EXTRA_ENABLED = "extra_enabled"
    const val EXTRA_STATE = "extra_state"

    const val NOTIFICATION_CHANNEL_ID = "pomodoro_timer_channel"
    const val NOTIFICATION_CHANNEL_NAME = "Pomodoro Timer"
    const val NOTIFICATION_ID = 101
}
