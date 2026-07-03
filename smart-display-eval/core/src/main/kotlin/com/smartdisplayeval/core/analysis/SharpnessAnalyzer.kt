package com.smartdisplayeval.core.analysis

import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.util.ImageOps

/**
 * Detects a loss of effective resolution / sharpness — the visible symptom of
 * an adaptive stream dropping to a low bitrate or resolution rung, or a decode
 * fallback.
 *
 * Sharpness is measured with the variance of the Laplacian (high-frequency
 * energy). Because the "correct" absolute sharpness depends heavily on content,
 * the analyzer learns a rolling peak of recent sharpness and flags a sustained
 * drop below a fraction of that peak. A one-off soft frame (e.g. a fade) does
 * not trip it; a persistent drop does.
 */
class SharpnessAnalyzer(private val config: Config = Config()) {

    data class Config(
        /** Fraction of the recent peak below which the image is "soft". */
        val dropFraction: Double = 0.45,
        /** Consecutive soft frames required before flagging. */
        val minConsecutive: Int = 3,
        /** Peak decays slowly so it tracks true scene sharpness capacity. */
        val peakDecay: Double = 0.98,
        /** Ignore near-black / flat frames where sharpness is meaningless. */
        val minPeak: Double = 25.0,
    )

    private var peak: Double = 0.0
    private var softRun: Int = 0

    fun reset() {
        peak = 0.0
        softRun = 0
    }

    fun analyze(frame: Frame): ArtifactEvent? {
        val sharp = ImageOps.laplacianVariance(frame)
        // Track a decaying peak of observed sharpness.
        peak = maxOf(sharp, peak * config.peakDecay)
        if (peak < config.minPeak) {
            softRun = 0
            return null
        }

        val ratio = sharp / peak
        if (ratio < config.dropFraction) {
            softRun++
        } else {
            softRun = 0
            return null
        }

        if (softRun < config.minConsecutive) return null

        val severity = ((config.dropFraction - ratio) / config.dropFraction).coerceIn(0.0, 1.0)
        return ArtifactEvent(
            type = ArtifactType.LOW_RESOLUTION,
            timestampMs = frame.timestampMs,
            frameIndex = frame.index,
            severity = severity,
            confidence = 0.7,
            detail = "sharpness=%.1f peak=%.1f ratio=%.2f run=%d".format(sharp, peak, ratio, softRun),
        )
    }
}
