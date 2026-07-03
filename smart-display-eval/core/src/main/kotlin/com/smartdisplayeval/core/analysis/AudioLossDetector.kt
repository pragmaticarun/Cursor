package com.smartdisplayeval.core.analysis

import com.smartdisplayeval.core.frame.Frame

/**
 * Detects audio dropouts: the audio level falling to (near) silence for a
 * sustained period while the picture indicates content is actually playing.
 *
 * Requiring "picture is moving" avoids flagging legitimately silent moments
 * (paused video, a quiet menu). A dropout is only emitted once, when the
 * silent stretch first crosses [Config.minSilenceMs]; if it continues, the
 * event is not repeated until audio returns and drops again.
 */
class AudioLossDetector(private val config: Config = Config()) {

    data class Config(
        /** Normalised RMS at/below which audio counts as silent. */
        val silenceRms: Double = 0.02,
        /** Minimum duration of silence before it is a dropout. */
        val minSilenceMs: Long = 400,
        /** Only treat silence as suspicious when motion exceeds this. */
        val activeMotion: Double = 0.01,
    )

    private var silenceStartMs: Long = -1
    private var flagged: Boolean = false

    fun reset() {
        silenceStartMs = -1
        flagged = false
    }

    /**
     * @param motion normalised motion for this frame (from the scene classifier),
     *               used to decide whether the picture is actively playing.
     */
    fun analyze(frame: Frame, motion: Double): ArtifactEvent? {
        val audio = frame.audio ?: return null // no audio channel -> nothing to judge
        val silent = audio.rms <= config.silenceRms
        val active = motion >= config.activeMotion

        if (silent && active) {
            if (silenceStartMs < 0) silenceStartMs = frame.timestampMs
            val elapsed = frame.timestampMs - silenceStartMs
            if (elapsed >= config.minSilenceMs && !flagged) {
                flagged = true
                val severity = (elapsed.toDouble() / (config.minSilenceMs * 3)).coerceIn(0.4, 1.0)
                return ArtifactEvent(
                    type = ArtifactType.AUDIO_LOSS,
                    timestampMs = silenceStartMs,
                    frameIndex = frame.index,
                    severity = severity,
                    confidence = 0.75,
                    detail = "silence for ${elapsed}ms while picture active (rms=%.3f)".format(audio.rms),
                )
            }
        } else {
            // Audio returned (or picture stopped) -> reset so a later dropout re-fires.
            silenceStartMs = -1
            flagged = false
        }
        return null
    }
}
