package com.smartdisplayeval.core.analysis

import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.util.ImageOps

/**
 * Classifies each incoming frame as [SceneType.STATIC], [SceneType.UI_MOTION]
 * or [SceneType.VIDEO].
 *
 * The classifier is stateful: it keeps the previous frame so it can measure
 * temporal motion and how coherent (globally rigid) that motion is. The core
 * intuitions:
 *
 *  - Almost no change -> STATIC.
 *  - Change that is well explained by a single global shift, over a crisp,
 *    low-entropy image -> UI_MOTION (a list/menu scrolling or panning).
 *  - Change that is locally varied (does not reduce to one shift) over a
 *    high-entropy, textured image -> VIDEO.
 *
 * Thresholds are exposed via [Config] so a caller can tune per device/optics.
 */
class SceneClassifier(private val config: Config = Config()) {

    data class Config(
        /** Below this normalised motion the scene is considered static. */
        val staticMotion: Double = 0.006,
        /** Global-shift coherence above this leans towards UI motion. */
        val uiCoherence: Double = 0.55,
        /** Luma entropy (bits) above this leans towards video. */
        val videoEntropy: Double = 6.3,
        /** Edge density above this leans towards UI (crisp graphics/text). */
        val uiEdgeDensity: Double = 0.10,
        /** Radius (px, coarse grid) for global motion search. */
        val motionSearchRadius: Int = 6,
    )

    private var prev: Frame? = null

    /** Reset internal state (e.g. between scenarios). */
    fun reset() {
        prev = null
    }

    fun classify(frame: Frame): SceneClassification {
        val previous = prev
        prev = frame

        val edge = ImageOps.edgeDensity(frame)
        val entropy = ImageOps.lumaEntropy(frame)

        if (previous == null || previous.width != frame.width || previous.height != frame.height) {
            return SceneClassification(
                frameIndex = frame.index,
                timestampMs = frame.timestampMs,
                type = SceneType.UNKNOWN,
                confidence = 0.0,
                motion = 0.0,
                motionCoherence = 0.0,
                edgeDensity = edge,
                textureEntropy = entropy,
            )
        }

        val mc = ImageOps.motionCoherence(previous, frame, config.motionSearchRadius)
        val motion = mc.motion

        if (motion < config.staticMotion) {
            // Confidence grows as motion falls further below the threshold.
            val conf = (1.0 - motion / config.staticMotion).coerceIn(0.3, 0.99)
            return SceneClassification(
                frame.index, frame.timestampMs, SceneType.STATIC, conf,
                motion, mc.coherence, edge, entropy,
            )
        }

        // Weighted vote between UI and VIDEO from independent cues.
        var uiScore = 0.0
        var videoScore = 0.0

        // Cue 1: motion coherence.
        if (mc.coherence >= config.uiCoherence) uiScore += (mc.coherence - config.uiCoherence) + 0.2
        else videoScore += (config.uiCoherence - mc.coherence) + 0.2

        // Cue 2: texture entropy.
        if (entropy >= config.videoEntropy) videoScore += (entropy - config.videoEntropy) / 2.0
        else uiScore += (config.videoEntropy - entropy) / 4.0

        // Cue 3: edge density (crisp graphics -> UI).
        if (edge >= config.uiEdgeDensity) uiScore += (edge - config.uiEdgeDensity)
        else videoScore += 0.05

        val type = if (uiScore >= videoScore) SceneType.UI_MOTION else SceneType.VIDEO
        val total = uiScore + videoScore
        val confidence = if (total <= 0.0) 0.5 else (max(uiScore, videoScore) / total).coerceIn(0.5, 0.99)

        return SceneClassification(
            frame.index, frame.timestampMs, type, confidence,
            motion, mc.coherence, edge, entropy,
        )
    }

    private fun max(a: Double, b: Double) = if (a >= b) a else b
}
