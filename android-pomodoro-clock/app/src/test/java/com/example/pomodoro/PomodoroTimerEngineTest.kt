package com.example.pomodoro

import org.junit.Assert.assertEquals
import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Test

class PomodoroTimerEngineTest {

    private val engine = PomodoroTimerEngine()

    @Test
    fun tick_decrements_when_running() {
        val state = PomodoroState(isRunning = true, remainingSeconds = 10)

        val next = engine.tick(state)

        assertEquals(9, next.remainingSeconds)
        assertTrue(next.isRunning)
    }

    @Test
    fun work_completion_moves_to_short_break_and_counts_session() {
        val state = PomodoroState(
            phase = PomodoroPhase.WORK,
            isRunning = true,
            remainingSeconds = 1,
            completedWorkSessions = 0,
            workSessionsSinceLongBreak = 0,
            shortBreakDurationMinutes = 5,
            cyclesBeforeLongBreak = 4
        )

        val next = engine.tick(state)

        assertEquals(PomodoroPhase.SHORT_BREAK, next.phase)
        assertEquals(5 * 60, next.remainingSeconds)
        assertEquals(1, next.completedWorkSessions)
        assertEquals(1, next.workSessionsSinceLongBreak)
    }

    @Test
    fun fourth_work_completion_moves_to_long_break() {
        val state = PomodoroState(
            phase = PomodoroPhase.WORK,
            isRunning = true,
            remainingSeconds = 1,
            completedWorkSessions = 3,
            workSessionsSinceLongBreak = 3,
            longBreakDurationMinutes = 20,
            cyclesBeforeLongBreak = 4
        )

        val next = engine.tick(state)

        assertEquals(PomodoroPhase.LONG_BREAK, next.phase)
        assertEquals(20 * 60, next.remainingSeconds)
        assertEquals(4, next.completedWorkSessions)
        assertEquals(0, next.workSessionsSinceLongBreak)
    }

    @Test
    fun disabling_autostart_pauses_on_phase_transition() {
        val state = PomodoroState(
            phase = PomodoroPhase.WORK,
            isRunning = true,
            remainingSeconds = 1,
            autoStartNextPhase = false
        )

        val next = engine.tick(state)

        assertFalse(next.isRunning)
    }
}
