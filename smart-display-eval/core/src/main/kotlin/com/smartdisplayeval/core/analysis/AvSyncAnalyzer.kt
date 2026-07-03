package com.smartdisplayeval.core.analysis

import kotlin.math.abs
import kotlin.math.sqrt

/** Result of audio/video synchronisation estimation. */
data class AvSyncReport(
    /** Estimated offset in ms; positive means audio lags the picture. */
    val offsetMs: Long,
    /** Peak normalised cross-correlation, 0..1. */
    val correlation: Double,
    /** Confidence that the estimate is meaningful, 0..1. */
    val confidence: Double,
    /** Number of time bins used. */
    val bins: Int,
)

/**
 * Estimates audio/video sync by cross-correlating two "onset" envelopes over
 * time: the audio level envelope and the visual-change (motion) envelope.
 *
 * The idea: events that produce a visible change (a cut, an impact, a UI action
 * with a sound) also produce an audio transient. If audio and video are in
 * sync, those envelopes line up at lag 0. A consistent lag that maximises the
 * correlation is the AV offset. Positive offset => audio arrives after the
 * matching picture change (audio delayed).
 */
class AvSyncAnalyzer(private val config: Config = Config()) {

    data class Config(
        /** Time-bin width for resampling the envelopes. */
        val binMs: Long = 33,
        /** Maximum absolute lag to search. */
        val maxLagMs: Long = 400,
        /** Minimum bins with signal before a result is trustworthy. */
        val minActiveBins: Int = 6,
    )

    private data class Sample(val t: Long, val audio: Double, val motion: Double)

    private val samples = ArrayList<Sample>()

    fun reset() = samples.clear()

    fun add(timestampMs: Long, audioRms: Double, motion: Double) {
        samples.add(Sample(timestampMs, audioRms, motion))
    }

    fun summarize(): AvSyncReport? {
        if (samples.size < config.minActiveBins) return null
        val start = samples.first().t
        val end = samples.last().t
        val binCount = ((end - start) / config.binMs + 1).toInt()
        if (binCount < config.minActiveBins) return null

        val audioEnv = DoubleArray(binCount)
        val videoEnv = DoubleArray(binCount)
        val counts = IntArray(binCount)
        for (s in samples) {
            val b = ((s.t - start) / config.binMs).toInt().coerceIn(0, binCount - 1)
            audioEnv[b] += s.audio
            videoEnv[b] += s.motion
            counts[b]++
        }
        for (i in 0 until binCount) {
            if (counts[i] > 0) {
                val c = counts[i].toDouble()
                audioEnv[i] = audioEnv[i] / c
                videoEnv[i] = videoEnv[i] / c
            }
        }

        // Convert to onset (positive change) envelopes: sync is driven by the
        // moments things start, not steady levels.
        val audioOnset = onset(audioEnv)
        val videoOnset = onset(videoEnv)

        val activeBins = videoOnset.count { it > 1e-6 }
        if (activeBins < config.minActiveBins) return null

        val maxLagBins = (config.maxLagMs / config.binMs).toInt()
        var bestLag = 0
        var bestCorr = -2.0
        for (lag in -maxLagBins..maxLagBins) {
            val c = normalisedCorrelation(videoOnset, audioOnset, lag)
            if (c > bestCorr) {
                bestCorr = c
                bestLag = lag
            }
        }
        val correlation = bestCorr.coerceIn(0.0, 1.0)
        val confidence = (correlation * (activeBins.toDouble() / (activeBins + 8))).coerceIn(0.0, 1.0)
        return AvSyncReport(
            offsetMs = bestLag * config.binMs,
            correlation = correlation,
            confidence = confidence,
            bins = binCount,
        )
    }

    /** Emits an AV_SYNC artifact if the summarised offset exceeds [maxAbsMs]. */
    fun artifactIfExceeds(maxAbsMs: Long, atTimestampMs: Long): ArtifactEvent? {
        val r = summarize() ?: return null
        if (abs(r.offsetMs) <= maxAbsMs || r.confidence < 0.3) return null
        val severity = ((abs(r.offsetMs) - maxAbsMs).toDouble() / (maxAbsMs * 3 + 1)).coerceIn(0.3, 1.0)
        return ArtifactEvent(
            type = ArtifactType.AV_SYNC,
            timestampMs = atTimestampMs,
            frameIndex = -1,
            severity = severity,
            confidence = r.confidence,
            detail = "audio offset ${r.offsetMs}ms (corr=%.2f)".format(r.correlation),
        )
    }

    private fun onset(env: DoubleArray): DoubleArray {
        val out = DoubleArray(env.size)
        for (i in 1 until env.size) {
            val d = env[i] - env[i - 1]
            out[i] = if (d > 0) d else 0.0
        }
        return out
    }

    private fun normalisedCorrelation(a: DoubleArray, b: DoubleArray, lag: Int): Double {
        var sum = 0.0
        var na = 0.0
        var nb = 0.0
        for (i in a.indices) {
            val j = i + lag
            if (j < 0 || j >= b.size) continue
            sum += a[i] * b[j]
            na += a[i] * a[i]
            nb += b[j] * b[j]
        }
        val denom = sqrt(na * nb)
        return if (denom < 1e-9) 0.0 else sum / denom
    }
}
