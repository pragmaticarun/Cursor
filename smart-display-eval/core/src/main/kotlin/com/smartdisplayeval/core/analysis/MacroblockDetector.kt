package com.smartdisplayeval.core.analysis

import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.util.ImageOps

/**
 * Detects macroblocking: blocky compression artifacts aligned to the codec's
 * DCT grid (8x8 for most H.264/H.265/VP9 content).
 *
 * Uses [ImageOps.blockiness], the ratio of luma discontinuity on the block grid
 * versus inside blocks. On clean content the ratio hovers near 1; when blocking
 * appears the periodic grid edges push it well above 1. To avoid firing on
 * naturally grid-aligned graphics, the detector tracks a short rolling baseline
 * and flags a *rise* above that baseline as well as an absolute floor.
 */
class MacroblockDetector(private val config: Config = Config()) {

    data class Config(
        val blockSize: Int = 8,
        /** Absolute blockiness ratio above which blocking is plausible. */
        val absoluteThreshold: Double = 1.35,
        /** Ratio relative to the rolling baseline that counts as a rise. */
        val relativeRise: Double = 1.25,
        /** Ratio that saturates severity to 1.0. */
        val severeThreshold: Double = 2.2,
        /** Rolling baseline smoothing factor (0..1); higher adapts faster. */
        val baselineAlpha: Double = 0.05,
    )

    private var baseline: Double = -1.0

    fun reset() {
        baseline = -1.0
    }

    /** Returns an event if this frame shows macroblocking, else null. */
    fun analyze(frame: Frame): ArtifactEvent? {
        val ratio = ImageOps.blockiness(frame, config.blockSize)
        if (baseline < 0) baseline = ratio

        val rises = ratio >= baseline * config.relativeRise
        val absolute = ratio >= config.absoluteThreshold

        // Update baseline slowly, and only towards lower/steady values so a
        // sustained artifact does not quietly become the new "normal".
        if (ratio <= baseline) {
            baseline += config.baselineAlpha * (ratio - baseline)
        } else {
            baseline += (config.baselineAlpha * 0.25) * (ratio - baseline)
        }

        if (!(rises && absolute)) return null

        val span = (config.severeThreshold - config.absoluteThreshold).coerceAtLeast(1e-6)
        val severity = ((ratio - config.absoluteThreshold) / span).coerceIn(0.0, 1.0)
        val confidence = ((ratio / config.absoluteThreshold) - 1.0).coerceIn(0.2, 0.95)
        return ArtifactEvent(
            type = ArtifactType.MACROBLOCKING,
            timestampMs = frame.timestampMs,
            frameIndex = frame.index,
            severity = severity,
            confidence = confidence,
            detail = "blockiness ratio=%.2f baseline=%.2f".format(ratio, baseline),
        )
    }
}
