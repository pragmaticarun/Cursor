package com.voicechat.app

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.view.WindowManager
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.voicechat.app.databinding.ActivityMainBinding
import org.webrtc.IceCandidate
import org.webrtc.PeerConnection
import org.webrtc.SessionDescription

/**
 * Main activity for the one-to-one voice chat app.
 *
 * Flow:
 * 1. User enters a signaling server URL and a room ID
 * 2. User taps "Join Room" to connect
 * 3. When two users are in the same room, the initiator creates an offer
 * 4. The peer responds with an answer
 * 5. ICE candidates are exchanged for NAT traversal
 * 6. Voice chat begins
 */
class MainActivity : AppCompatActivity(),
    WebRTCManager.WebRTCListener,
    SignalingClient.SignalingListener {

    companion object {
        private const val TAG = "MainActivity"
        private const val PERMISSION_REQUEST_CODE = 100
    }

    private lateinit var binding: ActivityMainBinding
    private lateinit var webRTCManager: WebRTCManager
    private lateinit var signalingClient: SignalingClient
    private lateinit var audioManagerHelper: AudioManagerHelper
    private lateinit var callTimer: CallTimer

    private var currentRoomId: String = ""
    private var isInitiator = false
    private var isInCall = false

    // ========== Lifecycle ==========

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Keep screen on during calls
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)

        initializeComponents()
        setupUI()
        checkPermissions()
    }

    override fun onDestroy() {
        super.onDestroy()
        endCall()
        signalingClient.disconnect()
        webRTCManager.dispose()
    }

    // ========== Initialization ==========

    private fun initializeComponents() {
        webRTCManager = WebRTCManager(this, this)
        signalingClient = SignalingClient(this)
        audioManagerHelper = AudioManagerHelper(this)
        callTimer = CallTimer { duration ->
            runOnUiThread {
                binding.tvCallDuration.text = duration
            }
        }

        // Initialize WebRTC
        webRTCManager.initialize()
    }

    private fun setupUI() {
        // Initially show the connect screen, hide the call screen
        showConnectScreen()

        // Join room button
        binding.btnJoinRoom.setOnClickListener {
            val serverUrl = binding.etServerUrl.text.toString().trim()
            val roomId = binding.etRoomId.text.toString().trim()

            if (serverUrl.isEmpty()) {
                binding.tilServerUrl.error = "Server URL is required"
                return@setOnClickListener
            }
            if (roomId.isEmpty()) {
                binding.tilRoomId.error = "Room ID is required"
                return@setOnClickListener
            }

            binding.tilServerUrl.error = null
            binding.tilRoomId.error = null

            currentRoomId = roomId
            connectToRoom(serverUrl, roomId)
        }

        // Mute button
        binding.btnMute.setOnClickListener {
            if (isInCall) {
                val isMuted = webRTCManager.toggleMute()
                updateMuteButton(isMuted)
            }
        }

        // Speaker button
        binding.btnSpeaker.setOnClickListener {
            if (isInCall) {
                val isSpeakerOn = audioManagerHelper.toggleSpeaker()
                updateSpeakerButton(isSpeakerOn)
            }
        }

        // End call button
        binding.btnEndCall.setOnClickListener {
            endCall()
        }

        // Disconnect button (in connect screen when connected but waiting)
        binding.btnDisconnect.setOnClickListener {
            signalingClient.leaveRoom(currentRoomId)
            signalingClient.disconnect()
            showConnectScreen()
        }
    }

    // ========== Permission Handling ==========

    private fun checkPermissions() {
        val permissions = mutableListOf(Manifest.permission.RECORD_AUDIO)

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            permissions.add(Manifest.permission.BLUETOOTH_CONNECT)
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            permissions.add(Manifest.permission.POST_NOTIFICATIONS)
        }

        val deniedPermissions = permissions.filter {
            ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }

        if (deniedPermissions.isNotEmpty()) {
            ActivityCompat.requestPermissions(
                this,
                deniedPermissions.toTypedArray(),
                PERMISSION_REQUEST_CODE
            )
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)

        if (requestCode == PERMISSION_REQUEST_CODE) {
            val audioGranted = grantResults.isNotEmpty() &&
                grantResults[0] == PackageManager.PERMISSION_GRANTED

            if (!audioGranted) {
                Toast.makeText(
                    this,
                    "Microphone permission is required for voice chat",
                    Toast.LENGTH_LONG
                ).show()
            }
        }
    }

    // ========== Connection Logic ==========

    private fun connectToRoom(serverUrl: String, roomId: String) {
        showConnectingState()
        signalingClient.connect(serverUrl)
        // Room join happens in onConnected callback
    }

    private fun startCall() {
        webRTCManager.createPeerConnection()
        audioManagerHelper.startAudioCall()

        if (isInitiator) {
            Log.d(TAG, "Creating offer as initiator")
            webRTCManager.createOffer { sdp ->
                signalingClient.sendOffer(sdp, currentRoomId)
            }
        }

        // Start foreground service
        val serviceIntent = Intent(this, VoiceChatService::class.java)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            startForegroundService(serviceIntent)
        } else {
            startService(serviceIntent)
        }
    }

    private fun endCall() {
        if (isInCall) {
            isInCall = false
            callTimer.stop()
            audioManagerHelper.stopAudioCall()

            signalingClient.leaveRoom(currentRoomId)
            signalingClient.disconnect()
            webRTCManager.dispose()

            // Stop foreground service
            stopService(Intent(this, VoiceChatService::class.java))

            // Re-initialize for next call
            webRTCManager = WebRTCManager(this, this)
            webRTCManager.initialize()

            runOnUiThread {
                showConnectScreen()
            }
        }
    }

    // ========== UI State Management ==========

    private fun showConnectScreen() {
        runOnUiThread {
            binding.connectLayout.visibility = View.VISIBLE
            binding.waitingLayout.visibility = View.GONE
            binding.callLayout.visibility = View.GONE

            binding.btnJoinRoom.isEnabled = true
            binding.btnJoinRoom.text = "Join Room"
            binding.tvStatus.text = "Enter details to start a voice chat"

            // Reset call UI
            binding.tvCallDuration.text = "00:00"
            updateMuteButton(false)
            updateSpeakerButton(false)
        }
    }

    private fun showConnectingState() {
        runOnUiThread {
            binding.btnJoinRoom.isEnabled = false
            binding.btnJoinRoom.text = "Connecting..."
            binding.tvStatus.text = "Connecting to server..."
        }
    }

    private fun showWaitingScreen() {
        runOnUiThread {
            binding.connectLayout.visibility = View.GONE
            binding.waitingLayout.visibility = View.VISIBLE
            binding.callLayout.visibility = View.GONE

            binding.tvWaitingStatus.text = "Waiting for peer to join room..."
            binding.tvRoomIdDisplay.text = "Room: $currentRoomId"
            binding.waitingProgress.visibility = View.VISIBLE
        }
    }

    private fun showCallScreen() {
        runOnUiThread {
            binding.connectLayout.visibility = View.GONE
            binding.waitingLayout.visibility = View.GONE
            binding.callLayout.visibility = View.VISIBLE

            binding.tvCallStatus.text = "Connected"
            binding.tvCallRoomId.text = "Room: $currentRoomId"

            // Animate the call controls in
            binding.callControlsLayout.alpha = 0f
            binding.callControlsLayout.animate()
                .alpha(1f)
                .setDuration(300)
                .start()
        }
    }

    private fun updateMuteButton(isMuted: Boolean) {
        runOnUiThread {
            if (isMuted) {
                binding.btnMute.setIconResource(android.R.drawable.ic_lock_silent_mode)
                binding.btnMute.text = "Unmute"
                binding.btnMute.setBackgroundColor(
                    ContextCompat.getColor(this, com.google.android.material.R.color.design_default_color_error)
                )
            } else {
                binding.btnMute.setIconResource(android.R.drawable.ic_btn_speak_now)
                binding.btnMute.text = "Mute"
                binding.btnMute.setBackgroundColor(
                    ContextCompat.getColor(this, R.color.control_button_bg)
                )
            }
        }
    }

    private fun updateSpeakerButton(isSpeakerOn: Boolean) {
        runOnUiThread {
            if (isSpeakerOn) {
                binding.btnSpeaker.text = "Earpiece"
                binding.btnSpeaker.setBackgroundColor(
                    ContextCompat.getColor(this, R.color.speaker_active)
                )
            } else {
                binding.btnSpeaker.text = "Speaker"
                binding.btnSpeaker.setBackgroundColor(
                    ContextCompat.getColor(this, R.color.control_button_bg)
                )
            }
        }
    }

    private fun showToast(message: String) {
        runOnUiThread {
            Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
        }
    }

    // ========== WebRTC Callbacks ==========

    override fun onIceCandidateGenerated(candidate: IceCandidate) {
        signalingClient.sendIceCandidate(candidate, currentRoomId)
    }

    override fun onConnectionStateChanged(state: PeerConnection.PeerConnectionState) {
        Log.d(TAG, "WebRTC connection state: $state")
        runOnUiThread {
            when (state) {
                PeerConnection.PeerConnectionState.CONNECTING -> {
                    binding.tvCallStatus.text = "Connecting..."
                }
                PeerConnection.PeerConnectionState.CONNECTED -> {
                    binding.tvCallStatus.text = "Connected"
                }
                PeerConnection.PeerConnectionState.DISCONNECTED -> {
                    binding.tvCallStatus.text = "Reconnecting..."
                }
                PeerConnection.PeerConnectionState.FAILED -> {
                    binding.tvCallStatus.text = "Connection failed"
                }
                else -> {}
            }
        }
    }

    override fun onCallConnected() {
        Log.d(TAG, "Call connected!")
        isInCall = true
        runOnUiThread {
            showCallScreen()
            callTimer.start()
        }
    }

    override fun onCallDisconnected() {
        Log.d(TAG, "Call disconnected")
        if (isInCall) {
            runOnUiThread {
                showToast("Call ended")
                endCall()
            }
        }
    }

    override fun onError(message: String) {
        Log.e(TAG, "WebRTC error: $message")
        showToast("Error: $message")
    }

    // ========== Signaling Callbacks ==========

    override fun onConnected() {
        Log.d(TAG, "Connected to signaling server, joining room: $currentRoomId")
        signalingClient.joinRoom(currentRoomId)
    }

    override fun onRoomJoined(roomId: String, isInitiator: Boolean) {
        Log.d(TAG, "Joined room: $roomId, isInitiator: $isInitiator")
        this.isInitiator = isInitiator

        if (isInitiator) {
            // We're the first in the room, wait for peer
            showWaitingScreen()
        } else {
            // Peer is already in the room, start the call
            runOnUiThread {
                showCallScreen()
                binding.tvCallStatus.text = "Connecting..."
            }
            startCall()
        }
    }

    override fun onOfferReceived(sdp: SessionDescription) {
        Log.d(TAG, "Offer received")
        webRTCManager.setRemoteDescription(sdp) {
            webRTCManager.createAnswer { answer ->
                signalingClient.sendAnswer(answer, currentRoomId)
            }
        }
    }

    override fun onAnswerReceived(sdp: SessionDescription) {
        Log.d(TAG, "Answer received")
        webRTCManager.setRemoteDescription(sdp)
    }

    override fun onIceCandidateReceived(candidate: IceCandidate) {
        Log.d(TAG, "ICE candidate received")
        webRTCManager.addIceCandidate(candidate)
    }

    override fun onPeerJoined() {
        Log.d(TAG, "Peer joined the room")
        showToast("Peer joined!")

        if (isInitiator) {
            // Start the call now that peer has joined
            startCall()
        }
    }

    override fun onPeerLeft() {
        Log.d(TAG, "Peer left the room")
        showToast("Peer left the call")
        endCall()
    }

    override fun onDisconnected() {
        Log.d(TAG, "Disconnected from signaling server")
        if (isInCall) {
            endCall()
        }
    }
}
