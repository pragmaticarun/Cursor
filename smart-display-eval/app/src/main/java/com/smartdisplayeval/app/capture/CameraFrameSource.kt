package com.smartdisplayeval.app.capture

import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import com.smartdisplayeval.core.control.FrameSource
import com.smartdisplayeval.core.frame.Frame
import java.util.concurrent.LinkedBlockingQueue
import java.util.concurrent.TimeUnit
import java.util.concurrent.atomic.AtomicInteger

/**
 * Bridges CameraX (push-based) to the core [FrameSource] (pull-based).
 *
 * Each analysed camera frame's luma (Y) plane is nearest-neighbour downsampled
 * to a small analysis resolution and paired with the latest audio level. Frames
 * are handed to the eval via a small bounded queue; if analysis briefly falls
 * behind, the newest frame replaces the oldest so we always reason over
 * near-real-time content rather than a growing backlog.
 *
 * Bind this as the analyzer of an [ImageAnalysis] use case, then pass the same
 * instance to the `ScenarioRunner` as its `FrameSource`.
 */
class CameraFrameSource(
    private val targetWidth: Int = 160,
    private val targetHeight: Int = 90,
    private val audioProbe: AudioLevelProbe? = null,
) : FrameSource, ImageAnalysis.Analyzer {

    private val queue = LinkedBlockingQueue<Frame>(4)
    private val index = AtomicInteger(0)
    private val startNs = System.nanoTime()

    @Volatile private var closed = false

    override fun analyze(image: ImageProxy) {
        try {
            if (closed) return
            val frame = convert(image)
            if (!queue.offer(frame)) {
                queue.poll() // drop oldest, keep freshest
                queue.offer(frame)
            }
        } finally {
            image.close()
        }
    }

    override fun next(): Frame? {
        while (!closed) {
            val f = queue.poll(750, TimeUnit.MILLISECONDS)
            if (f != null) return f
        }
        return queue.poll()
    }

    override fun close() {
        closed = true
    }

    private fun convert(image: ImageProxy): Frame {
        val yPlane = image.planes[0]
        val buffer = yPlane.buffer
        val rowStride = yPlane.rowStride
        val pixelStride = yPlane.pixelStride
        val srcW = image.width
        val srcH = image.height

        val luma = IntArray(targetWidth * targetHeight)
        for (ty in 0 until targetHeight) {
            val sy = (ty.toLong() * srcH / targetHeight).toInt().coerceIn(0, srcH - 1)
            val rowBase = sy * rowStride
            val outBase = ty * targetWidth
            for (tx in 0 until targetWidth) {
                val sx = (tx.toLong() * srcW / targetWidth).toInt().coerceIn(0, srcW - 1)
                val pos = rowBase + sx * pixelStride
                luma[outBase + tx] = buffer.get(pos).toInt() and 0xFF
            }
        }

        val tMs = (System.nanoTime() - startNs) / 1_000_000
        return Frame(
            index = index.getAndIncrement(),
            timestampMs = tMs,
            width = targetWidth,
            height = targetHeight,
            luma = luma,
            audio = audioProbe?.current(),
        )
    }
}
