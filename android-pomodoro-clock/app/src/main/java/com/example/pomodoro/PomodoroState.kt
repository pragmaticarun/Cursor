package com.example.pomodoro

enum class PomodoroPhase {
    WORK,
    SHORT_BREAK,
    LONG_BREAK
}

data class PomodoroState(
    val phase: PomodoroPhase = PomodoroPhase.WORK,
    val isRunning: Boolean = false,
    val remainingSeconds: Int = 25 * 60,
    val completedWorkSessions: Int = 0,
    val workSessionsSinceLongBreak: Int = 0,
    val workDurationMinutes: Int = 25,
    val shortBreakDurationMinutes: Int = 5,
    val longBreakDurationMinutes: Int = 15,
    val cyclesBeforeLongBreak: Int = 4,
    val autoStartNextPhase: Boolean = true
) {
    fun formattedRemainingTime(): String {
        val minutes = remainingSeconds / 60
        val seconds = remainingSeconds % 60
        return "%02d:%02d".format(minutes, seconds)
    }

    fun progressPercent(): Int {
        val total = durationFor(phase) * 60
        if (total <= 0) return 0
        val elapsed = (total - remainingSeconds).coerceIn(0, total)
        return (elapsed * 100) / total
    }

    fun progressFraction(): Float = progressPercent() / 100f

    fun currentPhaseDurationMinutes(): Int = durationFor(phase)

    private fun durationFor(currentPhase: PomodoroPhase): Int = when (currentPhase) {
        PomodoroPhase.WORK -> workDurationMinutes
        PomodoroPhase.SHORT_BREAK -> shortBreakDurationMinutes
        PomodoroPhase.LONG_BREAK -> longBreakDurationMinutes
    }
}
