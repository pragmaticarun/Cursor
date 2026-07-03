package com.smartdisplayeval.core.pipeline

import com.smartdisplayeval.core.analysis.ArtifactEvent
import com.smartdisplayeval.core.analysis.AudioLossDetector
import com.smartdisplayeval.core.analysis.AvSyncAnalyzer
import com.smartdisplayeval.core.analysis.AvSyncReport
import com.smartdisplayeval.core.analysis.BlackFrameDetector
import com.smartdisplayeval.core.analysis.FluidityAnalyzer
import com.smartdisplayeval.core.analysis.FluidityReport
import com.smartdisplayeval.core.analysis.MacroblockDetector
import com.smartdisplayeval.core.analysis.SceneClassification
import com.smartdisplayeval.core.analysis.SceneClassifier
import com.smartdisplayeval.core.analysis.SceneType
import com.smartdisplayeval.core.analysis.SharpnessAnalyzer
import com.smartdisplayeval.core.frame.Frame

/** A per-frame analysis result: the scene decision plus any artifacts found. */
data class FrameResult(
    val scene: SceneClassification,
    val artifacts: List<ArtifactEvent>,
)

/**
 * Runs every analyzer over a stream of frames, in order, one frame at a time.
 *
 * This is the near-real-time heart of the eval: for each captured frame it
 * classifies the scene and runs the artifact detectors, returning any events
 * immediately (each already carries the timestamp at which it occurred).
 * Analyzers that need a window (AV sync) accumulate here and are summarised at
 * the end via [finish].
 *
 * The pipeline is single-threaded and allocation-light so it can keep up with a
 * live capture feed on device.
 */
class FrameAnalysisPipeline(
    private val scene: SceneClassifier = SceneClassifier(),
    private val macroblock: MacroblockDetector = MacroblockDetector(),
    private val sharpness: SharpnessAnalyzer = SharpnessAnalyzer(),
    private val audioLoss: AudioLossDetector = AudioLossDetector(),
    private val blackFrame: BlackFrameDetector = BlackFrameDetector(),
    private val fluidity: FluidityAnalyzer = FluidityAnalyzer(),
    private val avSync: AvSyncAnalyzer = AvSyncAnalyzer(),
) {
    private val allArtifacts = ArrayList<ArtifactEvent>()
    private val sceneCounts = HashMap<SceneType, Int>()
    private var frameCount = 0

    val artifacts: List<ArtifactEvent> get() = allArtifacts

    fun reset() {
        scene.reset()
        macroblock.reset()
        sharpness.reset()
        audioLoss.reset()
        blackFrame.reset()
        fluidity.reset()
        avSync.reset()
        allArtifacts.clear()
        sceneCounts.clear()
        frameCount = 0
    }

    /** Process a single frame and return its result. */
    fun onFrame(frame: Frame): FrameResult {
        frameCount++
        val classification = scene.classify(frame)
        sceneCounts[classification.type] = (sceneCounts[classification.type] ?: 0) + 1

        val found = ArrayList<ArtifactEvent>(2)

        blackFrame.analyze(frame)?.let { found.add(it) }

        // Macroblocking and resolution only make sense on picture content, so
        // gate them on video (or ambiguous motion), not crisp UI.
        val onVideo = classification.type == SceneType.VIDEO
        if (onVideo) {
            macroblock.analyze(frame)?.let { found.add(it) }
            sharpness.analyze(frame)?.let { found.add(it) }
        }

        fluidity.analyze(frame.timestampMs, classification.motion, classification.type, frame.index)
            ?.let { found.add(it) }

        audioLoss.analyze(frame, classification.motion)?.let { found.add(it) }

        frame.audio?.let { avSync.add(frame.timestampMs, it.rms, classification.motion) }

        allArtifacts.addAll(found)
        return FrameResult(classification, found)
    }

    /**
     * Finalise window-based analysis. Call once after the last frame.
     *
     * @param avSyncMaxMs if non-null, emit an AV_SYNC artifact when the measured
     *                    offset exceeds this threshold.
     */
    fun finish(avSyncMaxMs: Long? = null, lastTimestampMs: Long = 0): PipelineSummary {
        val fluidityReport = fluidity.summarize()
        val avSyncReport = avSync.summarize()
        if (avSyncMaxMs != null) {
            avSync.artifactIfExceeds(avSyncMaxMs, lastTimestampMs)?.let { allArtifacts.add(it) }
        }
        return PipelineSummary(
            frameCount = frameCount,
            sceneCounts = HashMap(sceneCounts),
            fluidity = fluidityReport,
            avSync = avSyncReport,
            artifacts = ArrayList(allArtifacts),
        )
    }

    /** Fraction of processed frames classified as [type]. */
    fun sceneFraction(type: SceneType): Double {
        if (frameCount == 0) return 0.0
        return (sceneCounts[type] ?: 0).toDouble() / frameCount
    }
}

/** Aggregate results after a run. */
data class PipelineSummary(
    val frameCount: Int,
    val sceneCounts: Map<SceneType, Int>,
    val fluidity: FluidityReport,
    val avSync: AvSyncReport?,
    val artifacts: List<ArtifactEvent>,
)
