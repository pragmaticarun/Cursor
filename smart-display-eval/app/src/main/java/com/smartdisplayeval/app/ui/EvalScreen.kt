package com.smartdisplayeval.app.ui

import androidx.camera.view.PreviewView
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.DropdownMenu
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.compose.ui.viewinterop.AndroidView
import com.smartdisplayeval.app.ConnectionState
import com.smartdisplayeval.app.EvalViewModel
import com.smartdisplayeval.app.scenario.FireTvScenarios
import com.smartdisplayeval.core.analysis.ArtifactEvent
import com.smartdisplayeval.core.analysis.SceneType
import com.smartdisplayeval.core.control.FrameSource
import com.smartdisplayeval.core.report.EvalReport
import com.smartdisplayeval.core.scenario.Scenario

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun EvalScreen(
    viewModel: EvalViewModel,
    frameSource: FrameSource,
    onPreviewViewCreated: (PreviewView) -> Unit,
) {
    val state by viewModel.uiState.collectAsState()
    val scenarios = remember { FireTvScenarios.all }
    var selected by remember { mutableStateOf(scenarios.first()) }

    Scaffold(
        topBar = { TopAppBar(title = { Text("Smart Display Eval") }) },
    ) { padding ->
        Column(
            modifier = Modifier
                .padding(padding)
                .padding(16.dp)
                .fillMaxSize()
                .verticalScroll(rememberScrollState()),
            verticalArrangement = Arrangement.spacedBy(12.dp),
        ) {
            CameraPreview(onPreviewViewCreated)

            ConnectionCard(
                host = state.host,
                connection = state.connection,
                onHostChange = viewModel::setHost,
                onConnect = viewModel::connect,
            )

            ScenarioPicker(
                scenarios = scenarios,
                selected = selected,
                enabled = !state.running,
                onSelected = { selected = it },
            )

            Button(
                onClick = { viewModel.run(selected, frameSource) },
                enabled = state.connection == ConnectionState.Connected && !state.running,
                modifier = Modifier.fillMaxWidth(),
            ) {
                Text(if (state.running) "Running…" else "Run scenario")
            }

            state.message?.let {
                Text(it, style = MaterialTheme.typography.bodySmall, color = MaterialTheme.colorScheme.secondary)
            }

            if (state.running) {
                LiveStatus(step = state.currentStep, scene = state.currentScene, artifacts = state.liveArtifacts)
            }

            state.report?.let { ReportCard(it) }
        }
    }
}

@Composable
private fun CameraPreview(onPreviewViewCreated: (PreviewView) -> Unit) {
    Card(shape = RoundedCornerShape(16.dp)) {
        AndroidView(
            modifier = Modifier
                .fillMaxWidth()
                .height(200.dp),
            factory = { ctx ->
                PreviewView(ctx).also(onPreviewViewCreated)
            },
        )
    }
}

@Composable
private fun ConnectionCard(
    host: String,
    connection: ConnectionState,
    onHostChange: (String) -> Unit,
    onConnect: () -> Unit,
) {
    Card {
        Column(Modifier.padding(16.dp), verticalArrangement = Arrangement.spacedBy(8.dp)) {
            Text("Fire TV Stick (ADB over Wi‑Fi)", style = MaterialTheme.typography.titleMedium)
            Row(verticalAlignment = Alignment.CenterVertically) {
                OutlinedTextField(
                    value = host,
                    onValueChange = onHostChange,
                    label = { Text("IP address") },
                    singleLine = true,
                    modifier = Modifier.weight(1f),
                )
                Spacer(Modifier.width(12.dp))
                OutlinedButton(onClick = onConnect, enabled = connection != ConnectionState.Connecting) {
                    Text(
                        when (connection) {
                            ConnectionState.Connected -> "Reconnect"
                            ConnectionState.Connecting -> "…"
                            else -> "Connect"
                        },
                    )
                }
            }
            StatusChip(connection)
        }
    }
}

@Composable
private fun StatusChip(connection: ConnectionState) {
    val (label, color) = when (connection) {
        ConnectionState.Connected -> "Connected" to MaterialTheme.colorScheme.secondary
        ConnectionState.Connecting -> "Connecting…" to MaterialTheme.colorScheme.tertiary
        ConnectionState.Error -> "Error" to MaterialTheme.colorScheme.error
        ConnectionState.Disconnected -> "Not connected" to MaterialTheme.colorScheme.outline
    }
    Box(
        Modifier
            .background(color.copy(alpha = 0.15f), RoundedCornerShape(8.dp))
            .padding(horizontal = 10.dp, vertical = 4.dp),
    ) {
        Text(label, color = color, style = MaterialTheme.typography.labelMedium)
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
private fun ScenarioPicker(
    scenarios: List<Scenario>,
    selected: Scenario,
    enabled: Boolean,
    onSelected: (Scenario) -> Unit,
) {
    var expanded by remember { mutableStateOf(false) }
    Card {
        Column(Modifier.padding(16.dp), verticalArrangement = Arrangement.spacedBy(4.dp)) {
            Text("Scenario", style = MaterialTheme.typography.titleMedium)
            OutlinedButton(onClick = { expanded = true }, enabled = enabled) {
                Text(selected.name)
            }
            DropdownMenu(expanded = expanded, onDismissRequest = { expanded = false }) {
                scenarios.forEach { s ->
                    DropdownMenuItem(
                        text = { Text(s.name) },
                        onClick = { onSelected(s); expanded = false },
                    )
                }
            }
            Text(selected.description, style = MaterialTheme.typography.bodySmall)
        }
    }
}

@Composable
private fun LiveStatus(step: String?, scene: SceneType, artifacts: List<ArtifactEvent>) {
    Card {
        Column(Modifier.padding(16.dp), verticalArrangement = Arrangement.spacedBy(6.dp)) {
            Text("Live", style = MaterialTheme.typography.titleMedium)
            step?.let { Text("Step: $it") }
            Text("Scene: $scene", color = sceneColor(scene))
            Text("Artifacts so far: ${artifacts.size}")
            LazyColumn(Modifier.height(120.dp)) {
                items(artifacts.takeLast(20).reversed()) { a ->
                    Text(
                        "@${a.timestampMs}ms  ${a.type}  sev=${"%.2f".format(a.severity)}",
                        fontFamily = FontFamily.Monospace,
                        fontSize = 12.sp,
                    )
                }
            }
        }
    }
}

@Composable
private fun ReportCard(report: EvalReport) {
    Card(colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant)) {
        Column(Modifier.padding(16.dp), verticalArrangement = Arrangement.spacedBy(6.dp)) {
            Text("Report", style = MaterialTheme.typography.titleLarge)
            Text(report.scenarioName, style = MaterialTheme.typography.titleSmall)
            Row {
                Text(
                    "Score ${"%.0f".format(report.score())}/100",
                    fontWeight = FontWeight.Bold,
                    color = if (report.passed) MaterialTheme.colorScheme.secondary else MaterialTheme.colorScheme.error,
                )
                Spacer(Modifier.width(16.dp))
                Text(if (report.passed) "PASS" else "FAIL", fontWeight = FontWeight.Bold)
            }
            Text(
                "Fluidity: ${"%.0f".format(report.fluidity.measuredFps)} fps, " +
                    "smoothness ${"%.0f".format(report.fluidity.smoothness)}, " +
                    "stalls ${report.fluidity.stallCount}",
                style = MaterialTheme.typography.bodySmall,
            )
            report.avSync?.let {
                Text("AV sync offset ${it.offsetMs}ms", style = MaterialTheme.typography.bodySmall)
            }
            Spacer(Modifier.height(4.dp))
            report.stepResults.forEach { step ->
                Text(
                    "${if (step.passed) "✓" else "✗"} ${step.name}",
                    color = if (step.passed) MaterialTheme.colorScheme.secondary else MaterialTheme.colorScheme.error,
                    style = MaterialTheme.typography.bodyMedium,
                )
                step.expectationResults.forEach { e ->
                    Text(
                        "    ${if (e.passed) "ok" else "FAIL"} · ${e.description} → ${e.detail}",
                        fontFamily = FontFamily.Monospace,
                        fontSize = 11.sp,
                    )
                }
            }
            if (report.artifacts.isNotEmpty()) {
                Spacer(Modifier.height(4.dp))
                Text("Artifact timeline", style = MaterialTheme.typography.titleSmall)
                report.artifacts.sortedBy { it.timestampMs }.forEach { a ->
                    Text(
                        "@${a.timestampMs}ms ${a.type} sev=${"%.2f".format(a.severity)} · ${a.detail}",
                        fontFamily = FontFamily.Monospace,
                        fontSize = 11.sp,
                    )
                }
            }
        }
    }
}

private fun sceneColor(scene: SceneType): Color = when (scene) {
    SceneType.VIDEO -> Color(0xFF00B894)
    SceneType.UI_MOTION -> Color(0xFF6C5CE7)
    SceneType.STATIC -> Color(0xFF636E72)
    SceneType.UNKNOWN -> Color(0xFFB2BEC3)
}
