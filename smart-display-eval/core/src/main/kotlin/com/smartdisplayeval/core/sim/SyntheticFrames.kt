package com.smartdisplayeval.core.sim

import com.smartdisplayeval.core.frame.AudioSample
import com.smartdisplayeval.core.frame.Frame
import kotlin.math.sin
import kotlin.random.Random

/**
 * Generators for synthetic frames with known, controllable properties.
 *
 * These let the analyzers be exercised deterministically without any capture
 * hardware: we can produce a scrolling UI, natural-looking video, a frame
 * riddled with macroblocking, a soft/low-resolution frame, silence, etc., and
 * assert that the detectors respond correctly. They are also used by the demo
 * CLI to show the whole pipeline running end to end.
 */
object SyntheticFrames {

    const val W = 160
    const val H = 90

    /** A flat, near-static UI panel (a menu at rest) with a couple of crisp bars. */
    fun staticUi(index: Int, tMs: Long, jitter: Int = 0, rnd: Random = Random(index)): Frame {
        val luma = IntArray(W * H) { 40 }
        // A few crisp rectangles (menu tiles / text bars).
        drawRect(luma, 10, 10, 60, 24, 210)
        drawRect(luma, 10, 40, 90, 52, 180)
        drawRect(luma, 10, 60, 40, 72, 180)
        if (jitter > 0) addNoise(luma, jitter, rnd)
        return Frame(index, tMs, W, H, luma, AudioSample(rms = 0.05, peak = 0.1))
    }

    /**
     * A crisp UI scrolling coherently: a striped/tiled pattern shifted by a
     * constant offset each frame. High edge density, high motion coherence.
     */
    fun uiScroll(index: Int, tMs: Long, scrollY: Int): Frame {
        val luma = IntArray(W * H)
        for (y in 0 until H) {
            for (x in 0 until W) {
                val v = (((y + scrollY) / 12) % 2)
                val base = if (v == 0) 30 else 200
                // crisp vertical separators (list tiles)
                val sep = if (x % 40 < 2) 240 else base
                luma[y * W + x] = sep
            }
        }
        return Frame(index, tMs, W, H, luma, AudioSample(rms = 0.05, peak = 0.1))
    }

    /**
     * Natural-looking "video": a smooth luminance field that evolves over time
     * with fine texture (grain). Locally varied motion, high entropy, no global
     * shift -> should classify as VIDEO.
     */
    fun video(
        index: Int,
        tMs: Long,
        phase: Double,
        audioRms: Double = 0.4,
        rnd: Random = Random(index * 7919L),
    ): Frame {
        val luma = IntArray(W * H)
        for (y in 0 until H) {
            for (x in 0 until W) {
                val fx = x / W.toDouble()
                val fy = y / H.toDouble()
                val base = 128.0 +
                    60.0 * sin(6.0 * fx + phase) * sin(4.0 * fy - phase * 0.7) +
                    35.0 * sin(13.0 * fx + 2.0 * phase) * sin(11.0 * fy + phase)
                val grain = (rnd.nextInt(31) - 15)
                luma[y * W + x] = (base + grain).toInt().coerceIn(0, 255)
            }
        }
        return Frame(index, tMs, W, H, luma, AudioSample(rms = audioRms, peak = (audioRms + 0.2).coerceAtMost(1.0)))
    }

    /**
     * A heavily macroblocked video frame: the picture is quantised to constant
     * values over 8x8 blocks, producing strong periodic edges on the DCT grid.
     */
    fun macroblockedVideo(index: Int, tMs: Long, phase: Double): Frame {
        val luma = IntArray(W * H)
        val block = 8
        for (by in 0 until H step block) {
            for (bx in 0 until W step block) {
                val fx = bx / W.toDouble()
                val fy = by / H.toDouble()
                val base = 128.0 +
                    70.0 * sin(6.0 * fx + phase) * sin(4.0 * fy - phase) +
                    50.0 * sin(20.0 * fx) // strong block-to-block steps
                val v = base.toInt().coerceIn(0, 255)
                for (y in by until minOf(by + block, H)) {
                    for (x in bx until minOf(bx + block, W)) {
                        luma[y * W + x] = v
                    }
                }
            }
        }
        return Frame(index, tMs, W, H, luma, AudioSample(rms = 0.4, peak = 0.6))
    }

    /** Box-blur a frame to simulate a soft / low-resolution rung. */
    fun blur(frame: Frame, radius: Int = 3): Frame {
        val w = frame.width
        val h = frame.height
        val out = IntArray(w * h)
        for (y in 0 until h) {
            for (x in 0 until w) {
                var sum = 0
                var cnt = 0
                for (dy in -radius..radius) {
                    for (dx in -radius..radius) {
                        sum += frame.luma(x + dx, y + dy)
                        cnt++
                    }
                }
                out[y * w + x] = sum / cnt
            }
        }
        return Frame(frame.index, frame.timestampMs, w, h, out, frame.audio)
    }

    /** A near-black frame (signal loss / blank). */
    fun black(index: Int, tMs: Long): Frame =
        Frame(index, tMs, W, H, IntArray(W * H) { 3 }, AudioSample(rms = 0.0, peak = 0.0))

    /** Copy a frame but replace its audio measurement. */
    fun withAudio(frame: Frame, rms: Double, peak: Double = rms): Frame =
        Frame(frame.index, frame.timestampMs, frame.width, frame.height, frame.luma.copyOf(), AudioSample(rms, peak))

    private fun drawRect(luma: IntArray, x0: Int, y0: Int, x1: Int, y1: Int, v: Int) {
        for (y in y0 until y1) {
            for (x in x0 until x1) {
                if (x in 0 until W && y in 0 until H) luma[y * W + x] = v
            }
        }
    }

    private fun addNoise(luma: IntArray, amp: Int, rnd: Random) {
        for (i in luma.indices) {
            luma[i] = (luma[i] + rnd.nextInt(2 * amp + 1) - amp).coerceIn(0, 255)
        }
    }
}
