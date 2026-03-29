package com.voicechat.app

import android.content.Context
import android.media.AudioManager
import android.os.Build
import android.util.Log

/**
 * Helper class to manage audio routing and focus for voice calls.
 *
 * Handles:
 * - Audio focus requests
 * - Speaker/earpiece routing
 * - Audio mode management for voice communication
 */
class AudioManagerHelper(private val context: Context) {

    companion object {
        private const val TAG = "AudioManagerHelper"
    }

    private val audioManager = context.getSystemService(Context.AUDIO_SERVICE) as AudioManager
    private var previousAudioMode = AudioManager.MODE_NORMAL
    private var previousSpeakerState = false
    private var isSpeakerOn = false

    /**
     * Configure audio settings for voice call.
     */
    fun startAudioCall() {
        Log.d(TAG, "Configuring audio for voice call")

        // Save previous state
        previousAudioMode = audioManager.mode
        previousSpeakerState = audioManager.isSpeakerphoneOn

        // Request audio focus
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val focusRequest = android.media.AudioFocusRequest.Builder(
                AudioManager.AUDIOFOCUS_GAIN_TRANSIENT
            ).build()
            audioManager.requestAudioFocus(focusRequest)
        } else {
            @Suppress("DEPRECATION")
            audioManager.requestAudioFocus(
                null,
                AudioManager.STREAM_VOICE_CALL,
                AudioManager.AUDIOFOCUS_GAIN_TRANSIENT
            )
        }

        // Set audio mode for voice communication
        audioManager.mode = AudioManager.MODE_IN_COMMUNICATION

        // Default to earpiece
        audioManager.isSpeakerphoneOn = false
        isSpeakerOn = false

        Log.d(TAG, "Audio configured for voice call")
    }

    /**
     * Toggle between speaker and earpiece.
     * @return true if speaker is now enabled
     */
    fun toggleSpeaker(): Boolean {
        isSpeakerOn = !isSpeakerOn
        audioManager.isSpeakerphoneOn = isSpeakerOn
        Log.d(TAG, "Speaker ${if (isSpeakerOn) "ON" else "OFF"}")
        return isSpeakerOn
    }

    /**
     * Check if speaker is currently enabled.
     */
    fun isSpeakerEnabled(): Boolean = isSpeakerOn

    /**
     * Restore audio settings to pre-call state.
     */
    fun stopAudioCall() {
        Log.d(TAG, "Restoring audio settings")

        audioManager.mode = previousAudioMode
        audioManager.isSpeakerphoneOn = previousSpeakerState

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val focusRequest = android.media.AudioFocusRequest.Builder(
                AudioManager.AUDIOFOCUS_GAIN_TRANSIENT
            ).build()
            audioManager.abandonAudioFocusRequest(focusRequest)
        } else {
            @Suppress("DEPRECATION")
            audioManager.abandonAudioFocus(null)
        }

        Log.d(TAG, "Audio settings restored")
    }
}
