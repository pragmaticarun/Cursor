package com.smartdisplayeval.core.analysis

/**
 * High level classification of what the display is currently showing. The
 * distinction between UI motion and video is central to the eval: fluidity,
 * macroblocking, resolution and AV-sync checks only make sense (or use
 * different thresholds) depending on which of these is on screen.
 */
enum class SceneType {
    /** Little or no change between frames (a menu at rest, a paused image). */
    STATIC,

    /** Coherent, largely rigid motion: scrolling lists, panning menus, transitions. */
    UI_MOTION,

    /** Complex, locally varied motion consistent with decoded video playback. */
    VIDEO,

    /** Not enough information yet / ambiguous. */
    UNKNOWN,
}

/** Per-frame scene decision plus the features it was derived from. */
data class SceneClassification(
    val frameIndex: Int,
    val timestampMs: Long,
    val type: SceneType,
    val confidence: Double,
    val motion: Double,
    val motionCoherence: Double,
    val edgeDensity: Double,
    val textureEntropy: Double,
)

/** Categories of defect the eval can flag while observing a device. */
enum class ArtifactType {
    /** Blocky compression artifacts aligned to the codec's DCT grid. */
    MACROBLOCKING,

    /** Image noticeably softer / lower detail than expected (resolution downshift). */
    LOW_RESOLUTION,

    /** Audio and video drift out of sync beyond the allowed threshold. */
    AV_SYNC,

    /** Audio dropped out while the picture indicates active playback. */
    AUDIO_LOSS,

    /** A frame (or run of frames) failed to update while motion was expected. */
    FRAME_STALL,

    /** Irregular frame cadence / stutter during motion. */
    JUDDER,

    /** The screen went (near) black unexpectedly. */
    BLACK_FRAME,
}

/**
 * A single detected defect, tagged with the time it occurred so a report can
 * point a human (or an agent grading the run) at the exact moment.
 *
 * @param severity   0..1, how bad the instance is.
 * @param confidence 0..1, how sure the detector is.
 */
data class ArtifactEvent(
    val type: ArtifactType,
    val timestampMs: Long,
    val frameIndex: Int,
    val severity: Double,
    val confidence: Double,
    val detail: String,
)
