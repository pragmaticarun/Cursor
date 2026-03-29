package com.example.pomodoro

import org.junit.Assert.assertEquals
import org.junit.Test

class PomodoroStateTest {

    @Test
    fun formattedRemainingTime_returnsTwoDigitFormat() {
        val state = PomodoroState(remainingSeconds = 65)
        assertEquals("01:05", state.formattedRemainingTime())
    }

    @Test
    fun progressPercent_reflectsElapsedTime() {
        val state = PomodoroState(
            phase = PomodoroPhase.WORK,
            workDurationMinutes = 25,
            remainingSeconds = 10 * 60
        )

        assertEquals(60, state.progressPercent())
    }
}
