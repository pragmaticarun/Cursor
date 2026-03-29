package com.example.pomodoro

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.activity.viewModels
import androidx.compose.material3.Surface
import androidx.compose.ui.Modifier
import androidx.compose.foundation.layout.fillMaxSize
import com.example.pomodoro.ui.PomodoroApp
import com.example.pomodoro.ui.theme.PomodoroTheme

class MainActivity : ComponentActivity() {
    private val viewModel by viewModels<PomodoroViewModel>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            PomodoroTheme {
                Surface(modifier = Modifier.fillMaxSize()) {
                    PomodoroApp(viewModel = viewModel)
                }
            }
        }
    }
}
