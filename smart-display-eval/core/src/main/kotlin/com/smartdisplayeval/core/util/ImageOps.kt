package com.smartdisplayeval.core.util

import com.smartdisplayeval.core.frame.Frame
import kotlin.math.abs
import kotlin.math.ln
import kotlin.math.max
import kotlin.math.min
import kotlin.math.sqrt

/**
 * Low level, dependency-free image statistics used by the analyzers.
 *
 * Everything here operates on the grayscale [Frame.luma] plane. The functions
 * are intentionally simple and allocation-light so they can run per-frame in
 * near real time on a mobile device.
 */
object ImageOps {

    /**
     * Mean absolute temporal difference between two same-sized frames,
     * normalised to 0..1. This is the cheapest "how much changed" measure and
     * is the basis for motion / stall / scene decisions.
     */
    fun meanAbsDiff(a: Frame, b: Frame): Double {
        require(a.width == b.width && a.height == b.height) { "frame size mismatch" }
        val n = a.luma.size
        var sum = 0L
        for (i in 0 until n) sum += abs(a.luma[i] - b.luma[i])
        return (sum.toDouble() / n) / 255.0
    }

    /**
     * Fraction of pixels whose Sobel gradient magnitude exceeds [threshold]
     * (gradient measured in luma units). UI screens with crisp text/borders
     * produce high edge density; soft or noisy video produces less.
     */
    fun edgeDensity(f: Frame, threshold: Int = 48): Double {
        val w = f.width
        val h = f.height
        if (w < 3 || h < 3) return 0.0
        var count = 0
        var total = 0
        var y = 1
        while (y < h - 1) {
            var x = 1
            while (x < w - 1) {
                val gx = (f.luma(x + 1, y - 1) + 2 * f.luma(x + 1, y) + f.luma(x + 1, y + 1)) -
                    (f.luma(x - 1, y - 1) + 2 * f.luma(x - 1, y) + f.luma(x - 1, y + 1))
                val gy = (f.luma(x - 1, y + 1) + 2 * f.luma(x, y + 1) + f.luma(x + 1, y + 1)) -
                    (f.luma(x - 1, y - 1) + 2 * f.luma(x, y - 1) + f.luma(x + 1, y - 1))
                if (abs(gx) + abs(gy) >= threshold) count++
                total++
                x++
            }
            y++
        }
        return if (total == 0) 0.0 else count.toDouble() / total
    }

    /**
     * Shannon entropy (in bits, 0..8) of the luma histogram. Natural video has
     * a broad, high-entropy distribution; flat UI surfaces and solid colour
     * fields are low entropy.
     */
    fun lumaEntropy(f: Frame): Double {
        val hist = IntArray(256)
        for (v in f.luma) hist[v]++
        val n = f.luma.size.toDouble()
        var e = 0.0
        for (c in hist) {
            if (c == 0) continue
            val p = c / n
            e -= p * (ln(p) / LN2)
        }
        return e
    }

    /**
     * Variance of the Laplacian: a classic focus / sharpness measure. Higher
     * means more high-frequency detail (sharp). A sharp source that suddenly
     * goes soft (e.g. an adaptive stream dropping to a low bitrate/resolution)
     * shows up as a sharp drop in this value.
     */
    fun laplacianVariance(f: Frame): Double {
        val w = f.width
        val h = f.height
        if (w < 3 || h < 3) return 0.0
        var sum = 0.0
        var sumSq = 0.0
        var n = 0
        var y = 1
        while (y < h - 1) {
            var x = 1
            while (x < w - 1) {
                val lap = (4 * f.luma(x, y) - f.luma(x - 1, y) - f.luma(x + 1, y) -
                    f.luma(x, y - 1) - f.luma(x, y + 1)).toDouble()
                sum += lap
                sumSq += lap * lap
                n++
                x++
            }
            y++
        }
        if (n == 0) return 0.0
        val mean = sum / n
        return (sumSq / n) - mean * mean
    }

    /**
     * Blockiness metric on a fixed [blockSize] grid (8 for typical DCT codecs).
     *
     * Compares the average luma discontinuity that lands exactly on block
     * boundaries against the average discontinuity inside blocks. Compression
     * artifacts (macroblocking) create periodic step edges aligned to the codec
     * grid, so the ratio climbs well above 1 when blocking is present, while
     * detail that is not grid-aligned keeps it near 1.
     *
     * @return ratio boundaryDiff / interiorDiff (>= 0). ~1 means no blocking.
     */
    fun blockiness(f: Frame, blockSize: Int = 8): Double {
        val w = f.width
        val h = f.height
        if (w < blockSize * 2 || h < blockSize * 2) return 1.0

        var boundarySum = 0.0
        var boundaryCount = 0
        var interiorSum = 0.0
        var interiorCount = 0

        // Vertical neighbour differences (columns), classify by column index.
        var y = 0
        while (y < h) {
            var x = 1
            while (x < w) {
                val d = abs(f.luma(x, y) - f.luma(x - 1, y)).toDouble()
                if (x % blockSize == 0) {
                    boundarySum += d; boundaryCount++
                } else {
                    interiorSum += d; interiorCount++
                }
                x++
            }
            y++
        }
        // Horizontal neighbour differences (rows), classify by row index.
        var yy = 1
        while (yy < h) {
            var x = 0
            while (x < w) {
                val d = abs(f.luma(x, yy) - f.luma(x, yy - 1)).toDouble()
                if (yy % blockSize == 0) {
                    boundarySum += d; boundaryCount++
                } else {
                    interiorSum += d; interiorCount++
                }
                x++
            }
            yy++
        }

        val boundary = if (boundaryCount == 0) 0.0 else boundarySum / boundaryCount
        val interior = if (interiorCount == 0) 0.0 else interiorSum / interiorCount
        // Floor the interior term so perfectly flat blocks don't produce an
        // unbounded ratio, and cap the result to a readable range.
        return (boundary / max(interior, 0.5)).coerceAtMost(50.0)
    }

    /**
     * Estimates the dominant global translation between two frames by searching
     * a small integer shift window and returning a coherence score in 0..1.
     *
     * A high score means most of the temporal change is explained by a single
     * rigid shift, which is characteristic of UI panning/scrolling. Video
     * motion is locally varied and does not reduce to one global shift, so it
     * scores low.
     */
    fun motionCoherence(prev: Frame, cur: Frame, searchRadius: Int = 6): MotionCoherence {
        require(prev.width == cur.width && prev.height == cur.height) { "frame size mismatch" }
        val w = cur.width
        val h = cur.height
        // Work on a coarse grid for speed and noise robustness.
        val step = max(1, min(w, h) / 64)

        val baseSad = sad(prev, cur, 0, 0, step)
        if (baseSad < 1e-6) {
            // Effectively identical frames -> no motion, coherence undefined; treat as fully coherent (static).
            return MotionCoherence(coherence = 1.0, bestDx = 0, bestDy = 0, motion = 0.0)
        }

        var bestSad = baseSad
        var bestDx = 0
        var bestDy = 0
        var dy = -searchRadius
        while (dy <= searchRadius) {
            var dx = -searchRadius
            while (dx <= searchRadius) {
                if (dx == 0 && dy == 0) { dx++; continue }
                val s = sad(prev, cur, dx, dy, step)
                if (s < bestSad) {
                    bestSad = s; bestDx = dx; bestDy = dy
                }
                dx++
            }
            dy++
        }
        // Coherence: how much the best shift reduced the residual vs no shift.
        val coherence = ((baseSad - bestSad) / baseSad).coerceIn(0.0, 1.0)
        val motion = baseSad / 255.0
        return MotionCoherence(coherence, bestDx, bestDy, motion)
    }

    /** Sum of absolute differences of `cur` shifted by (dx,dy) against `prev`, sampled on a grid. */
    private fun sad(prev: Frame, cur: Frame, dx: Int, dy: Int, step: Int): Double {
        val w = cur.width
        val h = cur.height
        var sum = 0.0
        var n = 0
        var y = step
        while (y < h - step) {
            var x = step
            while (x < w - step) {
                val pv = prev.luma(x, y)
                val cv = cur.luma(x - dx, y - dy)
                sum += abs(pv - cv)
                n++
                x += step
            }
            y += step
        }
        return if (n == 0) 0.0 else sum / n
    }

    private const val LN2 = 0.6931471805599453
    private const val EPS = 1e-6

    /** Population standard deviation of a list. */
    fun stdDev(values: List<Double>): Double {
        if (values.size < 2) return 0.0
        val mean = values.average()
        var s = 0.0
        for (v in values) s += (v - mean) * (v - mean)
        return sqrt(s / values.size)
    }
}

/** Result of [ImageOps.motionCoherence]. */
data class MotionCoherence(
    val coherence: Double,
    val bestDx: Int,
    val bestDy: Int,
    /** Normalised base motion magnitude (0..1). */
    val motion: Double,
)
