package com.smartdisplayeval.core.frame

/**
 * A single captured frame of the observed display, reduced to the data the
 * analysis engine needs.
 *
 * The engine deliberately works on a grayscale (luma) plane plus an optional
 * audio measurement. This keeps the core free of any Android / capture-device
 * dependency: whether the frame originates from a phone camera pointed at a TV,
 * an HDMI capture dongle, or a recorded file, it is converted to this shape.
 *
 * @param index        monotonically increasing frame counter.
 * @param timestampMs  capture time relative to the start of the session, in ms.
 * @param width        luma plane width in pixels.
 * @param height       luma plane height in pixels.
 * @param luma         row-major grayscale values in the range 0..255,
 *                     length must equal width * height.
 * @param audio        optional audio measurement captured alongside the frame.
 */
class Frame(
    val index: Int,
    val timestampMs: Long,
    val width: Int,
    val height: Int,
    val luma: IntArray,
    val audio: AudioSample? = null,
) {
    init {
        require(width > 0 && height > 0) { "frame dimensions must be positive" }
        require(luma.size == width * height) {
            "luma length ${luma.size} != width*height ${width * height}"
        }
    }

    /** Luma value at (x, y), clamped to the frame bounds. */
    fun luma(x: Int, y: Int): Int {
        val cx = if (x < 0) 0 else if (x >= width) width - 1 else x
        val cy = if (y < 0) 0 else if (y >= height) height - 1 else y
        return luma[cy * width + cx]
    }

    /** Mean luma across the whole frame (0..255). */
    fun meanLuma(): Double {
        var sum = 0L
        for (v in luma) sum += v
        return sum.toDouble() / luma.size
    }
}

/**
 * A summarised audio measurement for the short window that overlaps a frame.
 *
 * We do not carry raw PCM into the core; a capture layer computes these
 * aggregate levels (cheap, done per-frame) and the analyzers reason over them.
 *
 * @param rms          normalised RMS level, 0.0 (silence) .. 1.0 (full scale).
 * @param peak         normalised peak level, 0.0 .. 1.0.
 * @param sampleRateHz nominal PCM sample rate the levels were derived from.
 */
data class AudioSample(
    val rms: Double,
    val peak: Double,
    val sampleRateHz: Int = 48_000,
)
