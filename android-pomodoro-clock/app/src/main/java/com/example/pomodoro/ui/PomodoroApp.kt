package com.example.pomodoro.ui

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.example.pomodoro.PomodoroPhase
import com.example.pomodoro.PomodoroViewModel

@Composable
fun PomodoroApp(viewModel: PomodoroViewModel) {
    val state by viewModel.state.collectAsStateWithLifecycle()

    var workDraft by remember(state.workDurationMinutes) { mutableStateOf(state.workDurationMinutes.toString()) }
    var shortBreakDraft by remember(state.shortBreakDurationMinutes) { mutableStateOf(state.shortBreakDurationMinutes.toString()) }
    var longBreakDraft by remember(state.longBreakDurationMinutes) { mutableStateOf(state.longBreakDurationMinutes.toString()) }

    LaunchedEffect(
        state.workDurationMinutes,
        state.shortBreakDurationMinutes,
        state.longBreakDurationMinutes
    ) {
        workDraft = state.workDurationMinutes.toString()
        shortBreakDraft = state.shortBreakDurationMinutes.toString()
        longBreakDraft = state.longBreakDurationMinutes.toString()
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(20.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        Text(
            text = "Pomodoro Clock",
            style = MaterialTheme.typography.headlineMedium,
            fontWeight = FontWeight.Bold
        )

        Card(modifier = Modifier.fillMaxWidth()) {
            Column(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                Text(
                    text = state.phase.label,
                    style = MaterialTheme.typography.titleMedium,
                    fontWeight = FontWeight.Medium
                )
                Text(
                    text = state.formattedRemainingTime(),
                    style = MaterialTheme.typography.displayMedium
                )
                Text(
                    text = "Progress: ${state.progressPercent()}%",
                    style = MaterialTheme.typography.bodyMedium
                )
                Text(
                    text = "Completed focus sessions: ${state.completedWorkSessions}",
                    style = MaterialTheme.typography.bodyMedium
                )
            }
        }

        Row(horizontalArrangement = Arrangement.spacedBy(12.dp)) {
            if (state.isRunning) {
                OutlinedButton(onClick = viewModel::pause) {
                    Text("Pause")
                }
            } else {
                Button(onClick = viewModel::start) {
                    Text("Start")
                }
            }

            OutlinedButton(onClick = viewModel::resetCurrentPhase) {
                Text("Reset")
            }
            OutlinedButton(onClick = viewModel::skipToNextPhase) {
                Text("Skip")
            }
        }

        Card(modifier = Modifier.fillMaxWidth()) {
            Column(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                Text(
                    text = "Session Durations (minutes)",
                    style = MaterialTheme.typography.titleSmall
                )

                DurationField(
                    label = "Focus",
                    value = workDraft,
                    onValueChange = { input ->
                        workDraft = input.filter { it.isDigit() }.take(2)
                        workDraft.toIntOrNull()?.let(viewModel::updateWorkDuration)
                    }
                )
                DurationField(
                    label = "Short Break",
                    value = shortBreakDraft,
                    onValueChange = { input ->
                        shortBreakDraft = input.filter { it.isDigit() }.take(2)
                        shortBreakDraft.toIntOrNull()?.let(viewModel::updateShortBreakDuration)
                    }
                )
                DurationField(
                    label = "Long Break",
                    value = longBreakDraft,
                    onValueChange = { input ->
                        longBreakDraft = input.filter { it.isDigit() }.take(2)
                        longBreakDraft.toIntOrNull()?.let(viewModel::updateLongBreakDuration)
                    }
                )

                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Column {
                        Text("Auto-start next session")
                        Text(
                            text = if (state.autoStartNextPhase) "Enabled" else "Disabled",
                            style = MaterialTheme.typography.bodySmall
                        )
                    }
                    Switch(
                        checked = state.autoStartNextPhase,
                        onCheckedChange = viewModel::setAutoStart
                    )
                }

                Text(
                    text = "Cycle pattern: Focus ×4 with short breaks, then a long break.",
                    style = MaterialTheme.typography.bodySmall
                )
            }
        }

        Spacer(modifier = Modifier.height(8.dp))
        Text(
            text = "Tip: keep the app open to let the in-app timer continue.",
            style = MaterialTheme.typography.bodySmall
        )
    }
}

@Composable
private fun DurationField(
    label: String,
    value: String,
    onValueChange: (String) -> Unit
) {
    OutlinedTextField(
        modifier = Modifier.fillMaxWidth(),
        value = value,
        onValueChange = onValueChange,
        label = { Text(label) },
        singleLine = true
    )
}

private val PomodoroPhase.label: String
    get() = when (this) {
        PomodoroPhase.WORK -> "Focus Time"
        PomodoroPhase.SHORT_BREAK -> "Short Break"
        PomodoroPhase.LONG_BREAK -> "Long Break"
    }
