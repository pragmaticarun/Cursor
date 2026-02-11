package com.voicechat.app

import android.content.Context
import android.util.Log
import org.webrtc.*

/**
 * Manages WebRTC peer connection for one-to-one voice chat.
 *
 * Handles:
 * - PeerConnection creation and lifecycle
 * - Audio track capture and playback
 * - ICE candidate exchange
 * - SDP offer/answer negotiation
 */
class WebRTCManager(
    private val context: Context,
    private val listener: WebRTCListener
) {
    companion object {
        private const val TAG = "WebRTCManager"

        // Public STUN/TURN servers for NAT traversal
        private val ICE_SERVERS = listOf(
            PeerConnection.IceServer.builder("stun:stun.l.google.com:19302").createIceServer(),
            PeerConnection.IceServer.builder("stun:stun1.l.google.com:19302").createIceServer(),
            PeerConnection.IceServer.builder("stun:stun2.l.google.com:19302").createIceServer(),
            PeerConnection.IceServer.builder("stun:stun3.l.google.com:19302").createIceServer(),
            PeerConnection.IceServer.builder("stun:stun4.l.google.com:19302").createIceServer()
        )
    }

    private var peerConnectionFactory: PeerConnectionFactory? = null
    private var peerConnection: PeerConnection? = null
    private var localAudioTrack: AudioTrack? = null
    private var audioSource: AudioSource? = null
    private var isMuted = false

    interface WebRTCListener {
        fun onIceCandidateGenerated(candidate: IceCandidate)
        fun onConnectionStateChanged(state: PeerConnection.PeerConnectionState)
        fun onCallConnected()
        fun onCallDisconnected()
        fun onError(message: String)
    }

    /**
     * Initialize the WebRTC peer connection factory and audio components.
     */
    fun initialize() {
        Log.d(TAG, "Initializing WebRTC...")

        // Initialize PeerConnectionFactory
        val initOptions = PeerConnectionFactory.InitializationOptions.builder(context)
            .setFieldTrials("")
            .setEnableInternalTracer(false)
            .createInitializationOptions()
        PeerConnectionFactory.initialize(initOptions)

        // Configure audio
        val audioDeviceModule = JavaAudioDeviceModule.builder(context)
            .setUseHardwareAcousticEchoCanceler(true)
            .setUseHardwareNoiseSuppressor(true)
            .createAudioDeviceModule()

        // Build factory with audio processing
        val options = PeerConnectionFactory.Options()
        peerConnectionFactory = PeerConnectionFactory.builder()
            .setOptions(options)
            .setAudioDeviceModule(audioDeviceModule)
            .createPeerConnectionFactory()

        Log.d(TAG, "WebRTC initialized successfully")
    }

    /**
     * Create a new peer connection and set up audio track.
     */
    fun createPeerConnection() {
        val factory = peerConnectionFactory ?: run {
            listener.onError("PeerConnectionFactory not initialized")
            return
        }

        val rtcConfig = PeerConnection.RTCConfiguration(ICE_SERVERS).apply {
            sdpSemantics = PeerConnection.SdpSemantics.UNIFIED_PLAN
            continualGatheringPolicy = PeerConnection.ContinualGatheringPolicy.GATHER_CONTINUALLY
            // Enable ICE candidate pooling for faster connections
            iceCandidatePoolSize = 5
        }

        peerConnection = factory.createPeerConnection(rtcConfig, createPeerConnectionObserver())

        if (peerConnection == null) {
            listener.onError("Failed to create PeerConnection")
            return
        }

        // Create and add local audio track
        setupLocalAudio(factory)

        Log.d(TAG, "PeerConnection created successfully")
    }

    /**
     * Set up local audio capture and add track to peer connection.
     */
    private fun setupLocalAudio(factory: PeerConnectionFactory) {
        val audioConstraints = MediaConstraints().apply {
            mandatory.add(MediaConstraints.KeyValuePair("googEchoCancellation", "true"))
            mandatory.add(MediaConstraints.KeyValuePair("googAutoGainControl", "true"))
            mandatory.add(MediaConstraints.KeyValuePair("googHighpassFilter", "true"))
            mandatory.add(MediaConstraints.KeyValuePair("googNoiseSuppression", "true"))
        }

        audioSource = factory.createAudioSource(audioConstraints)
        localAudioTrack = factory.createAudioTrack("local_audio_track", audioSource)
        localAudioTrack?.setEnabled(true)

        peerConnection?.addTrack(localAudioTrack, listOf("local_stream"))

        Log.d(TAG, "Local audio track added")
    }

    /**
     * Create an SDP offer (caller side).
     */
    fun createOffer(callback: (SessionDescription) -> Unit) {
        val constraints = MediaConstraints().apply {
            mandatory.add(MediaConstraints.KeyValuePair("OfferToReceiveAudio", "true"))
            mandatory.add(MediaConstraints.KeyValuePair("OfferToReceiveVideo", "false"))
        }

        peerConnection?.createOffer(object : SdpObserver {
            override fun onCreateSuccess(sdp: SessionDescription) {
                Log.d(TAG, "Offer created successfully")
                setLocalDescription(sdp) {
                    callback(sdp)
                }
            }

            override fun onCreateFailure(error: String) {
                Log.e(TAG, "Failed to create offer: $error")
                listener.onError("Failed to create offer: $error")
            }

            override fun onSetSuccess() {}
            override fun onSetFailure(error: String) {}
        }, constraints)
    }

    /**
     * Create an SDP answer (callee side).
     */
    fun createAnswer(callback: (SessionDescription) -> Unit) {
        val constraints = MediaConstraints().apply {
            mandatory.add(MediaConstraints.KeyValuePair("OfferToReceiveAudio", "true"))
            mandatory.add(MediaConstraints.KeyValuePair("OfferToReceiveVideo", "false"))
        }

        peerConnection?.createAnswer(object : SdpObserver {
            override fun onCreateSuccess(sdp: SessionDescription) {
                Log.d(TAG, "Answer created successfully")
                setLocalDescription(sdp) {
                    callback(sdp)
                }
            }

            override fun onCreateFailure(error: String) {
                Log.e(TAG, "Failed to create answer: $error")
                listener.onError("Failed to create answer: $error")
            }

            override fun onSetSuccess() {}
            override fun onSetFailure(error: String) {}
        }, constraints)
    }

    /**
     * Set the local session description.
     */
    private fun setLocalDescription(sdp: SessionDescription, onSuccess: () -> Unit) {
        peerConnection?.setLocalDescription(object : SdpObserver {
            override fun onSetSuccess() {
                Log.d(TAG, "Local description set successfully")
                onSuccess()
            }

            override fun onSetFailure(error: String) {
                Log.e(TAG, "Failed to set local description: $error")
                listener.onError("Failed to set local description: $error")
            }

            override fun onCreateSuccess(sdp: SessionDescription?) {}
            override fun onCreateFailure(error: String?) {}
        }, sdp)
    }

    /**
     * Set the remote session description received from the peer.
     */
    fun setRemoteDescription(sdp: SessionDescription, onSuccess: () -> Unit = {}) {
        peerConnection?.setRemoteDescription(object : SdpObserver {
            override fun onSetSuccess() {
                Log.d(TAG, "Remote description set successfully")
                onSuccess()
            }

            override fun onSetFailure(error: String) {
                Log.e(TAG, "Failed to set remote description: $error")
                listener.onError("Failed to set remote description: $error")
            }

            override fun onCreateSuccess(sdp: SessionDescription?) {}
            override fun onCreateFailure(error: String?) {}
        }, sdp)
    }

    /**
     * Add an ICE candidate received from the remote peer.
     */
    fun addIceCandidate(candidate: IceCandidate) {
        peerConnection?.addIceCandidate(candidate)
        Log.d(TAG, "ICE candidate added")
    }

    /**
     * Toggle microphone mute state.
     */
    fun toggleMute(): Boolean {
        isMuted = !isMuted
        localAudioTrack?.setEnabled(!isMuted)
        Log.d(TAG, "Microphone ${if (isMuted) "muted" else "unmuted"}")
        return isMuted
    }

    /**
     * Check if microphone is currently muted.
     */
    fun isMicMuted(): Boolean = isMuted

    /**
     * Toggle the speakerphone on/off.
     */
    fun setSpeakerEnabled(enabled: Boolean) {
        val audioManager = context.getSystemService(Context.AUDIO_SERVICE) as android.media.AudioManager
        audioManager.isSpeakerphoneOn = enabled
        Log.d(TAG, "Speaker ${if (enabled) "enabled" else "disabled"}")
    }

    /**
     * Clean up all WebRTC resources.
     */
    fun dispose() {
        Log.d(TAG, "Disposing WebRTC resources...")
        localAudioTrack?.dispose()
        audioSource?.dispose()
        peerConnection?.close()
        peerConnection?.dispose()
        peerConnectionFactory?.dispose()

        localAudioTrack = null
        audioSource = null
        peerConnection = null
        peerConnectionFactory = null

        Log.d(TAG, "WebRTC resources disposed")
    }

    /**
     * Create the PeerConnection observer that handles connection events.
     */
    private fun createPeerConnectionObserver(): PeerConnection.Observer {
        return object : PeerConnection.Observer {
            override fun onIceCandidate(candidate: IceCandidate) {
                Log.d(TAG, "ICE candidate generated: ${candidate.sdpMid}")
                listener.onIceCandidateGenerated(candidate)
            }

            override fun onConnectionChange(newState: PeerConnection.PeerConnectionState) {
                Log.d(TAG, "Connection state changed: $newState")
                listener.onConnectionStateChanged(newState)

                when (newState) {
                    PeerConnection.PeerConnectionState.CONNECTED -> {
                        listener.onCallConnected()
                    }
                    PeerConnection.PeerConnectionState.DISCONNECTED,
                    PeerConnection.PeerConnectionState.FAILED,
                    PeerConnection.PeerConnectionState.CLOSED -> {
                        listener.onCallDisconnected()
                    }
                    else -> {}
                }
            }

            override fun onSignalingChange(state: PeerConnection.SignalingState) {
                Log.d(TAG, "Signaling state: $state")
            }

            override fun onIceConnectionChange(state: PeerConnection.IceConnectionState) {
                Log.d(TAG, "ICE connection state: $state")
            }

            override fun onIceConnectionReceivingChange(receiving: Boolean) {
                Log.d(TAG, "ICE receiving: $receiving")
            }

            override fun onIceGatheringChange(state: PeerConnection.IceGatheringState) {
                Log.d(TAG, "ICE gathering state: $state")
            }

            override fun onIceCandidatesRemoved(candidates: Array<out IceCandidate>) {
                Log.d(TAG, "ICE candidates removed")
            }

            override fun onAddStream(stream: MediaStream) {
                Log.d(TAG, "Remote stream added with ${stream.audioTracks.size} audio tracks")
            }

            override fun onRemoveStream(stream: MediaStream) {
                Log.d(TAG, "Remote stream removed")
            }

            override fun onDataChannel(channel: DataChannel) {
                Log.d(TAG, "Data channel: ${channel.label()}")
            }

            override fun onRenegotiationNeeded() {
                Log.d(TAG, "Renegotiation needed")
            }

            override fun onAddTrack(receiver: RtpReceiver, streams: Array<out MediaStream>) {
                Log.d(TAG, "Track added: ${receiver.track()?.kind()}")
            }
        }
    }
}
