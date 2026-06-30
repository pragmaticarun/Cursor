package com.example.constitution.ui.theme

import android.app.Activity
import android.os.Build
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.material3.dynamicDarkColorScheme
import androidx.compose.material3.dynamicLightColorScheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext

private val LightColors = lightColorScheme(
    primary = ChakraNavy,
    onPrimary = Color.White,
    primaryContainer = Color(0xFFD7E2FF),
    onPrimaryContainer = Color(0xFF001A41),
    secondary = SaffronDark,
    onSecondary = Color.White,
    secondaryContainer = Color(0xFFFFE0B2),
    onSecondaryContainer = Color(0xFF3A2200),
    tertiary = IndiaGreen,
    onTertiary = Color.White,
    tertiaryContainer = Color(0xFFC8E6C9),
    onTertiaryContainer = Color(0xFF002106),
    background = LightBackground,
    onBackground = Color(0xFF1C2430),
    surface = LightSurface,
    onSurface = Color(0xFF1C2430),
    surfaceVariant = Color(0xFFEFE7DA),
    onSurfaceVariant = Color(0xFF51606F),
)

private val DarkColors = darkColorScheme(
    primary = ChakraNavyLight,
    onPrimary = Color(0xFF002F66),
    primaryContainer = Color(0xFF274777),
    onPrimaryContainer = Color(0xFFD7E2FF),
    secondary = Saffron,
    onSecondary = Color(0xFF4A2800),
    secondaryContainer = Color(0xFF6A3D00),
    onSecondaryContainer = Color(0xFFFFE0B2),
    tertiary = IndiaGreenLight,
    onTertiary = Color(0xFF003908),
    tertiaryContainer = Color(0xFF155216),
    onTertiaryContainer = Color(0xFFC8E6C9),
    background = DarkBackground,
    onBackground = Color(0xFFE8ECF3),
    surface = DarkSurface,
    onSurface = Color(0xFFE8ECF3),
    surfaceVariant = Color(0xFF2A3548),
    onSurfaceVariant = Color(0xFFA3AFC0),
)

@Composable
fun ConstitutionTheme(
    darkTheme: Boolean = isSystemInDarkTheme(),
    dynamicColor: Boolean = true,
    content: @Composable () -> Unit
) {
    val colorScheme = when {
        dynamicColor && Build.VERSION.SDK_INT >= Build.VERSION_CODES.S -> {
            val context = LocalContext.current
            if (darkTheme) dynamicDarkColorScheme(context) else dynamicLightColorScheme(context)
        }
        darkTheme -> DarkColors
        else -> LightColors
    }

    MaterialTheme(
        colorScheme = colorScheme,
        content = content
    )
}
