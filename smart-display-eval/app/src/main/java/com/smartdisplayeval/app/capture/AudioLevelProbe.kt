package com.smartdisplayeval.app.capture

import android.annotation.SuppressLint
import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import com.smartdisplayeval.core.frame.AudioSample
import kotlin.concurrent.thread
import kotlin.math.sqrt

/**
 * Continuously samples audio level from the microphone (pointed at the TV, or
 * fed from a line/HDMI audio source) and exposes the latest normalised RMS/peak
 * as an [AudioSample]. The eval's audio-loss and AV-sync analyzers consume these
 * per-frame measurements.
 *
 * Requires the RECORD_AUDIO permission to have been granted before [start].
 */
class AudioLevelProbe(private val sampleRateHz: Int = 44_100) {

    @Volatile private var running = false
    @Volatile private var latest = AudioSample(rms = 0.0, peak = 0.0, sampleRateHz = sampleRateHz)
    private var worker: Thread? = null

    fun current(): AudioSample = latest

    @SuppressLint("MissingPermission")
    fun start() {
        if (running) return
        running = true
        val minBuf = AudioRecord.getMinBufferSize(
            sampleRateHz,
            AudioFormat.CHANNEL_IN_MONO,
            AudioFormat.ENCODING_PCM_16BIT,
        ).coerceAtLeast(2048)

        worker = thread(name = "audio-level-probe", isDaemon = true) {
            val record = AudioRecord(
                MediaRecorder.AudioSource.MIC,
                sampleRateHz,
                AudioFormat.CHANNEL_IN_MONO,
                AudioFormat.ENCODING_PCM_16BIT,
                minBuf,
            )
            val buffer = ShortArray(minBuf / 2)
            try {
                record.startRecording()
                while (running) {
                    val n = record.read(buffer, 0, buffer.size)
                    if (n <= 0) continue
                    var sumSq = 0.0
                    var peak = 0
                    for (i in 0 until n) {
                        val s = buffer[i].toInt()
                        sumSq += (s * s).toDouble()
                        val a = if (s < 0) -s else s
                        if (a > peak) peak = a
                    }
                    val rms = sqrt(sumSq / n) / 32768.0
                    latest = AudioSample(
                        rms = rms.coerceIn(0.0, 1.0),
                        peak = (peak / 32768.0).coerceIn(0.0, 1.0),
                        sampleRateHz = sampleRateHz,
                    )
                }
            } catch (_: Throwable) {
                // Leave latest as the last good value; capture can proceed video-only.
            } finally {
                runCatching { record.stop() }
                runCatching { record.release() }
            }
        }
    }

    fun stop() {
        running = false
        worker?.join(500)
        worker = null
    }
}
