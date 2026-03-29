package com.example.pomodoro

class PomodoroTimerEngine {

    fun start(state: PomodoroState): PomodoroState = state.copy(isRunning = true)

    fun pause(state: PomodoroState): PomodoroState = state.copy(isRunning = false)

    fun resetCurrentPhase(state: PomodoroState): PomodoroState = state.copy(
        isRunning = false,
        remainingSeconds = state.currentPhaseDurationMinutes() * 60
    )

    fun skipToNextPhase(state: PomodoroState): PomodoroState {
        val (nextPhase, completedDelta, nextSinceLongBreak) = when (state.phase) {
            PomodoroPhase.WORK -> {
                val nextCount = state.workSessionsSinceLongBreak + 1
                if (nextCount >= state.cyclesBeforeLongBreak) {
                    Triple(PomodoroPhase.LONG_BREAK, 1, 0)
                } else {
                    Triple(PomodoroPhase.SHORT_BREAK, 1, nextCount)
                }
            }
            PomodoroPhase.SHORT_BREAK,
            PomodoroPhase.LONG_BREAK -> Triple(PomodoroPhase.WORK, 0, state.workSessionsSinceLongBreak)
        }

        return state.copy(
            phase = nextPhase,
            isRunning = false,
            completedWorkSessions = state.completedWorkSessions + completedDelta,
            workSessionsSinceLongBreak = nextSinceLongBreak,
            remainingSeconds = durationFor(nextPhase, state) * 60
        )
    }

    fun updateWorkDuration(state: PomodoroState, minutes: Int): PomodoroState {
        val clamped = minutes.coerceIn(MIN_DURATION_MINUTES, MAX_WORK_MINUTES)
        return state.copy(
            workDurationMinutes = clamped,
            remainingSeconds = if (!state.isRunning && state.phase == PomodoroPhase.WORK) {
                clamped * 60
            } else {
                state.remainingSeconds
            }
        )
    }

    fun updateShortBreakDuration(state: PomodoroState, minutes: Int): PomodoroState {
        val clamped = minutes.coerceIn(MIN_DURATION_MINUTES, MAX_BREAK_MINUTES)
        return state.copy(
            shortBreakDurationMinutes = clamped,
            remainingSeconds = if (!state.isRunning && state.phase == PomodoroPhase.SHORT_BREAK) {
                clamped * 60
            } else {
                state.remainingSeconds
            }
        )
    }

    fun updateLongBreakDuration(state: PomodoroState, minutes: Int): PomodoroState {
        val clamped = minutes.coerceIn(MIN_DURATION_MINUTES, MAX_BREAK_MINUTES)
        return state.copy(
            longBreakDurationMinutes = clamped,
            remainingSeconds = if (!state.isRunning && state.phase == PomodoroPhase.LONG_BREAK) {
                clamped * 60
            } else {
                state.remainingSeconds
            }
        )
    }

    fun setAutoStart(state: PomodoroState, enabled: Boolean): PomodoroState =
        state.copy(autoStartNextPhase = enabled)

    fun tick(state: PomodoroState): PomodoroState {
        if (!state.isRunning) return state
        if (state.remainingSeconds > 1) {
            return state.copy(remainingSeconds = state.remainingSeconds - 1)
        }
        return onPhaseCompleted(state)
    }

    private fun onPhaseCompleted(state: PomodoroState): PomodoroState {
        return when (state.phase) {
            PomodoroPhase.WORK -> {
                val completedSessions = state.completedWorkSessions + 1
                val sinceLongBreak = state.workSessionsSinceLongBreak + 1
                val nextPhase = if (sinceLongBreak >= state.cyclesBeforeLongBreak) {
                    PomodoroPhase.LONG_BREAK
                } else {
                    PomodoroPhase.SHORT_BREAK
                }
                val nextSinceLongBreak = if (nextPhase == PomodoroPhase.LONG_BREAK) 0 else sinceLongBreak
                state.copy(
                    phase = nextPhase,
                    completedWorkSessions = completedSessions,
                    workSessionsSinceLongBreak = nextSinceLongBreak,
                    remainingSeconds = durationFor(nextPhase, state) * 60,
                    isRunning = state.autoStartNextPhase
                )
            }
            PomodoroPhase.SHORT_BREAK,
            PomodoroPhase.LONG_BREAK -> state.copy(
                phase = PomodoroPhase.WORK,
                remainingSeconds = state.workDurationMinutes * 60,
                isRunning = state.autoStartNextPhase
            )
        }
    }

    private fun durationFor(phase: PomodoroPhase, state: PomodoroState): Int = when (phase) {
        PomodoroPhase.WORK -> state.workDurationMinutes
        PomodoroPhase.SHORT_BREAK -> state.shortBreakDurationMinutes
        PomodoroPhase.LONG_BREAK -> state.longBreakDurationMinutes
    }

    companion object {
        const val MIN_DURATION_MINUTES = 1
        const val MAX_WORK_MINUTES = 90
        const val MAX_BREAK_MINUTES = 60
    }
}
