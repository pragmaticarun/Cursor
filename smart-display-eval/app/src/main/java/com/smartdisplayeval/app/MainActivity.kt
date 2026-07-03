package com.smartdisplayeval.app

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.result.contract.ActivityResultContracts
import androidx.activity.viewModels
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import com.smartdisplayeval.app.capture.AudioLevelProbe
import com.smartdisplayeval.app.capture.CameraFrameSource
import com.smartdisplayeval.app.ui.EvalScreen
import com.smartdisplayeval.app.ui.SmartDisplayEvalTheme
import java.util.concurrent.Executors

/**
 * Single-activity host. Sets up the camera capture pipeline (a
 * [CameraFrameSource] fed by CameraX plus an [AudioLevelProbe]) and hands it to
 * the Compose [EvalScreen], which drives scenarios through [EvalViewModel].
 */
class MainActivity : ComponentActivity() {

    private val viewModel: EvalViewModel by viewModels()

    private val audioProbe = AudioLevelProbe()
    private val frameSource = CameraFrameSource(audioProbe = audioProbe)
    private val analysisExecutor = Executors.newSingleThreadExecutor()

    private var previewView: PreviewView? = null

    private val permissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestMultiplePermissions(),
    ) { grants ->
        if (grants[Manifest.permission.CAMERA] == true) {
            startCamera()
        }
        if (grants[Manifest.permission.RECORD_AUDIO] == true) {
            audioProbe.start()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            SmartDisplayEvalTheme {
                EvalScreen(
                    viewModel = viewModel,
                    frameSource = frameSource,
                    onPreviewViewCreated = { pv ->
                        previewView = pv
                        ensurePermissionsAndStart()
                    },
                )
            }
        }
    }

    private fun ensurePermissionsAndStart() {
        val needed = buildList {
            if (!granted(Manifest.permission.CAMERA)) add(Manifest.permission.CAMERA)
            if (!granted(Manifest.permission.RECORD_AUDIO)) add(Manifest.permission.RECORD_AUDIO)
        }
        if (needed.isEmpty()) {
            startCamera()
            audioProbe.start()
        } else {
            permissionLauncher.launch(needed.toTypedArray())
        }
    }

    private fun granted(permission: String): Boolean =
        ContextCompat.checkSelfPermission(this, permission) == PackageManager.PERMISSION_GRANTED

    private fun startCamera() {
        val pv = previewView ?: return
        val providerFuture = ProcessCameraProvider.getInstance(this)
        providerFuture.addListener({
            val provider = providerFuture.get()

            val preview = Preview.Builder().build().also {
                it.setSurfaceProvider(pv.surfaceProvider)
            }
            val analysis = ImageAnalysis.Builder()
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                .build()
                .also { it.setAnalyzer(analysisExecutor, frameSource) }

            // Rear camera by default: point the phone at the TV.
            val selector = CameraSelector.DEFAULT_BACK_CAMERA
            runCatching {
                provider.unbindAll()
                provider.bindToLifecycle(this, selector, preview, analysis)
            }
        }, ContextCompat.getMainExecutor(this))
    }

    override fun onDestroy() {
        super.onDestroy()
        audioProbe.stop()
        frameSource.close()
        analysisExecutor.shutdown()
    }
}
