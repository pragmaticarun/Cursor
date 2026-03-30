package com.example.pomodoro.ui.theme

import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import com.example.pomodoro.PomodoroPhase

val FocusColor = Color(0xFFFF6B6B)
val ShortBreakColor = Color(0xFF4ECDC4)
val LongBreakColor = Color(0xFF5C7AEA)

private val AppColorScheme = darkColorScheme()

@Composable
fun PomodoroTheme(
    content: @Composable () -> Unit
) {
    MaterialTheme(
        colorScheme = AppColorScheme,
        content = content
    )
}

fun PomodoroPhase.phaseColor(): Color = when (this) {
    PomodoroPhase.WORK -> FocusColor
    PomodoroPhase.SHORT_BREAK -> ShortBreakColor
    PomodoroPhase.LONG_BREAK -> LongBreakColor
}
