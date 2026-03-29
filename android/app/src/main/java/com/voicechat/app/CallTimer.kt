package com.voicechat.app

import android.os.Handler
import android.os.Looper
import android.os.SystemClock

/**
 * Utility class to track and display call duration.
 */
class CallTimer(
    private val onTick: (String) -> Unit
) {
    private val handler = Handler(Looper.getMainLooper())
    private var startTime = 0L
    private var isRunning = false

    private val tickRunnable = object : Runnable {
        override fun run() {
            if (isRunning) {
                val elapsed = SystemClock.elapsedRealtime() - startTime
                onTick(formatDuration(elapsed))
                handler.postDelayed(this, 1000)
            }
        }
    }

    fun start() {
        startTime = SystemClock.elapsedRealtime()
        isRunning = true
        handler.post(tickRunnable)
    }

    fun stop() {
        isRunning = false
        handler.removeCallbacks(tickRunnable)
    }

    fun reset() {
        stop()
        onTick("00:00")
    }

    private fun formatDuration(millis: Long): String {
        val totalSeconds = millis / 1000
        val minutes = totalSeconds / 60
        val seconds = totalSeconds % 60
        val hours = minutes / 60

        return if (hours > 0) {
            String.format("%02d:%02d:%02d", hours, minutes % 60, seconds)
        } else {
            String.format("%02d:%02d", minutes, seconds)
        }
    }
}
