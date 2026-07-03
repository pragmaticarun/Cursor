package com.smartdisplayeval.core.analysis

import com.smartdisplayeval.core.frame.Frame

/**
 * Flags unexpected black / signal-loss frames: a very low, very flat picture.
 * A single black frame during a transition is normal, so an event is emitted
 * only once per black stretch (edge-triggered).
 */
class BlackFrameDetector(private val config: Config = Config()) {

    data class Config(
        /** Mean luma at/below which the frame is "dark". */
        val maxMeanLuma: Double = 12.0,
    )

    private var inBlack = false

    fun reset() {
        inBlack = false
    }

    fun analyze(frame: Frame): ArtifactEvent? {
        val mean = frame.meanLuma()
        val dark = mean <= config.maxMeanLuma
        if (dark && !inBlack) {
            inBlack = true
            return ArtifactEvent(
                type = ArtifactType.BLACK_FRAME,
                timestampMs = frame.timestampMs,
                frameIndex = frame.index,
                severity = 0.6,
                confidence = 0.8,
                detail = "mean luma=%.1f".format(mean),
            )
        }
        if (!dark) inBlack = false
        return null
    }
}
