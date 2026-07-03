package com.smartdisplayeval.core.analysis

import com.smartdisplayeval.core.util.ImageOps
import kotlin.math.abs

/** Summary of motion fluidity over an observation window. */
data class FluidityReport(
    val frameCount: Int,
    /** Capture rate actually observed (frames per second). */
    val measuredFps: Double,
    /** Standard deviation of capture intervals in ms (capture jitter). */
    val captureJitterMs: Double,
    /** Frames that were part of an active-motion period. */
    val motionFrames: Int,
    /** Number of distinct freeze/stall events during motion. */
    val stallCount: Int,
    /** Longest single freeze during motion, in ms. */
    val longestStallMs: Long,
    /** 0..100; blends stall penalty and cadence regularity. 100 = perfectly fluid. */
    val smoothness: Double,
)

/**
 * Measures how smoothly on-screen motion plays back.
 *
 * Fed one sample per captured frame (timestamp + normalised motion + the scene
 * type), it distinguishes "should be moving" periods (UI transitions/scrolling
 * and video) from static ones, then looks for:
 *
 *  - **Freezes / stalls** — motion collapsing to ~0 for a sustained stretch
 *    while we are in an active-motion period. Each is emitted as a timestamped
 *    [ArtifactType.FRAME_STALL] event.
 *  - **Judder** — irregular frame-to-frame motion cadence during motion, folded
 *    into the smoothness score in [summarize].
 */
class FluidityAnalyzer(private val config: Config = Config()) {

    data class Config(
        /** Motion above this starts / sustains an active-motion period. */
        val motionOn: Double = 0.012,
        /** Motion at/below this counts as "no update this frame". */
        val motionOff: Double = 0.003,
        /** A low-motion stretch this long inside motion is a stall. */
        val stallMs: Long = 180,
        /** Active period is considered ended after this much quiet. */
        val settleMs: Long = 500,
    )

    private data class Sample(val t: Long, val motion: Double, val active: Boolean)

    private val samples = ArrayList<Sample>()

    // Stall tracking state.
    private var lowStartMs: Long = -1
    private var stallFlagged = false
    private var stallCount = 0
    private var longestStallMs = 0L
    private var lastActiveMotionMs = -1L
    private var lastActiveScene = SceneType.UNKNOWN

    fun reset() {
        samples.clear()
        lowStartMs = -1
        stallFlagged = false
        stallCount = 0
        longestStallMs = 0
        lastActiveMotionMs = -1
        lastActiveScene = SceneType.UNKNOWN
    }

    /** Feed one frame; returns a stall event if a freeze just crossed the threshold. */
    fun analyze(timestampMs: Long, motion: Double, scene: SceneType, frameIndex: Int): ArtifactEvent? {
        if (motion >= config.motionOn) {
            lastActiveMotionMs = timestampMs
            lastActiveScene = scene
        }
        // A freeze during playback classifies as STATIC (the picture stopped
        // changing), so we look at the scene that was active *before* motion
        // dropped. Freezes are only treated as stalls during VIDEO: a paused
        // scroll or a settled menu is expected to stop moving, but continuous
        // video playback should never freeze.
        val active = lastActiveScene == SceneType.VIDEO &&
            lastActiveMotionMs >= 0 &&
            (timestampMs - lastActiveMotionMs) <= config.settleMs

        samples.add(Sample(timestampMs, motion, active))

        var event: ArtifactEvent? = null
        if (active && motion <= config.motionOff) {
            if (lowStartMs < 0) lowStartMs = timestampMs
            val dur = timestampMs - lowStartMs
            if (dur > longestStallMs) longestStallMs = dur
            if (dur >= config.stallMs && !stallFlagged) {
                stallFlagged = true
                stallCount++
                event = ArtifactEvent(
                    type = ArtifactType.FRAME_STALL,
                    timestampMs = lowStartMs,
                    frameIndex = frameIndex,
                    severity = (dur.toDouble() / (config.stallMs * 4)).coerceIn(0.3, 1.0),
                    confidence = 0.7,
                    detail = "picture frozen ~${dur}ms during motion",
                )
            }
        } else {
            lowStartMs = -1
            stallFlagged = false
        }
        return event
    }

    fun summarize(): FluidityReport {
        val n = samples.size
        if (n < 2) {
            return FluidityReport(n, 0.0, 0.0, 0, stallCount, longestStallMs, 100.0)
        }
        val intervals = ArrayList<Double>(n - 1)
        for (i in 1 until n) intervals.add((samples[i].t - samples[i - 1].t).toDouble())
        val meanInterval = intervals.average()
        val measuredFps = if (meanInterval > 0) 1000.0 / meanInterval else 0.0
        val jitter = ImageOps.stdDev(intervals)

        // Cadence regularity during active-motion frames: variation of the
        // frame-to-frame motion signal. Smooth motion changes gradually; judder
        // shows up as large swings / periodic dips.
        val activeMotions = samples.filter { it.active }.map { it.motion }
        val motionFrames = activeMotions.size
        var judder = 0.0
        if (activeMotions.size >= 3) {
            val mean = activeMotions.average().coerceAtLeast(1e-6)
            var swing = 0.0
            for (i in 1 until activeMotions.size) swing += abs(activeMotions[i] - activeMotions[i - 1])
            judder = (swing / (activeMotions.size - 1)) / mean // normalised mean abs change
        }

        // Score: start at 100, penalise stalls and judder.
        val stallPenalty = (stallCount * 12).coerceAtMost(60)
        val judderPenalty = (judder * 45.0).coerceIn(0.0, 40.0)
        val smoothness = (100.0 - stallPenalty - judderPenalty).coerceIn(0.0, 100.0)

        return FluidityReport(
            frameCount = n,
            measuredFps = measuredFps,
            captureJitterMs = jitter,
            motionFrames = motionFrames,
            stallCount = stallCount,
            longestStallMs = longestStallMs,
            smoothness = smoothness,
        )
    }
}
