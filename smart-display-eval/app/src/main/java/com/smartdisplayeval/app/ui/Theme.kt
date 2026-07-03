package com.smartdisplayeval.app.ui

import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color

private val Purple = Color(0xFF6C5CE7)
private val Teal = Color(0xFF00B894)
private val Amber = Color(0xFFFDCB6E)

private val DarkColors = darkColorScheme(
    primary = Purple,
    secondary = Teal,
    tertiary = Amber,
)

private val LightColors = lightColorScheme(
    primary = Purple,
    secondary = Teal,
    tertiary = Amber,
)

@Composable
fun SmartDisplayEvalTheme(content: @Composable () -> Unit) {
    MaterialTheme(
        colorScheme = if (isSystemInDarkTheme()) DarkColors else LightColors,
        content = content,
    )
}
