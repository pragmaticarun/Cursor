package com.voicechat.app

import android.util.Log
import okhttp3.*
import org.json.JSONObject
import org.webrtc.IceCandidate
import org.webrtc.SessionDescription
import java.util.concurrent.TimeUnit

/**
 * WebSocket-based signaling client for exchanging SDP offers/answers
 * and ICE candidates between peers.
 *
 * The signaling server facilitates peer discovery and message relay
 * using a room-based model where two users join the same room.
 */
class SignalingClient(
    private val listener: SignalingListener
) {
    companion object {
        private const val TAG = "SignalingClient"
        private const val NORMAL_CLOSURE_STATUS = 1000
    }

    private var webSocket: WebSocket? = null
    private var isConnected = false
    private val client = OkHttpClient.Builder()
        .readTimeout(30, TimeUnit.SECONDS)
        .writeTimeout(30, TimeUnit.SECONDS)
        .pingInterval(15, TimeUnit.SECONDS)
        .retryOnConnectionFailure(true)
        .build()

    interface SignalingListener {
        fun onConnected()
        fun onRoomJoined(roomId: String, isInitiator: Boolean)
        fun onOfferReceived(sdp: SessionDescription)
        fun onAnswerReceived(sdp: SessionDescription)
        fun onIceCandidateReceived(candidate: IceCandidate)
        fun onPeerJoined()
        fun onPeerLeft()
        fun onDisconnected()
        fun onError(message: String)
    }

    /**
     * Connect to the signaling server.
     *
     * @param serverUrl WebSocket URL of the signaling server (e.g., ws://192.168.1.100:8080)
     */
    fun connect(serverUrl: String) {
        Log.d(TAG, "Connecting to signaling server: $serverUrl")

        val request = Request.Builder()
            .url(serverUrl)
            .build()

        webSocket = client.newWebSocket(request, object : WebSocketListener() {
            override fun onOpen(webSocket: WebSocket, response: Response) {
                Log.d(TAG, "Connected to signaling server")
                isConnected = true
                listener.onConnected()
            }

            override fun onMessage(webSocket: WebSocket, text: String) {
                handleMessage(text)
            }

            override fun onClosing(webSocket: WebSocket, code: Int, reason: String) {
                Log.d(TAG, "Connection closing: $reason")
                webSocket.close(NORMAL_CLOSURE_STATUS, null)
            }

            override fun onClosed(webSocket: WebSocket, code: Int, reason: String) {
                Log.d(TAG, "Connection closed: $reason")
                isConnected = false
                listener.onDisconnected()
            }

            override fun onFailure(webSocket: WebSocket, t: Throwable, response: Response?) {
                Log.e(TAG, "Connection failed: ${t.message}")
                isConnected = false
                listener.onError("Connection failed: ${t.message}")
            }
        })
    }

    /**
     * Join a voice chat room. If the room doesn't exist, it's created
     * and the caller becomes the initiator. If it exists, the caller
     * joins as the second peer.
     */
    fun joinRoom(roomId: String) {
        sendMessage(JSONObject().apply {
            put("type", "join")
            put("roomId", roomId)
        })
    }

    /**
     * Leave the current room.
     */
    fun leaveRoom(roomId: String) {
        sendMessage(JSONObject().apply {
            put("type", "leave")
            put("roomId", roomId)
        })
    }

    /**
     * Send an SDP offer to the peer.
     */
    fun sendOffer(sdp: SessionDescription, roomId: String) {
        sendMessage(JSONObject().apply {
            put("type", "offer")
            put("roomId", roomId)
            put("sdp", JSONObject().apply {
                put("type", sdp.type.canonicalForm())
                put("sdp", sdp.description)
            })
        })
    }

    /**
     * Send an SDP answer to the peer.
     */
    fun sendAnswer(sdp: SessionDescription, roomId: String) {
        sendMessage(JSONObject().apply {
            put("type", "answer")
            put("roomId", roomId)
            put("sdp", JSONObject().apply {
                put("type", sdp.type.canonicalForm())
                put("sdp", sdp.description)
            })
        })
    }

    /**
     * Send an ICE candidate to the peer.
     */
    fun sendIceCandidate(candidate: IceCandidate, roomId: String) {
        sendMessage(JSONObject().apply {
            put("type", "ice-candidate")
            put("roomId", roomId)
            put("candidate", JSONObject().apply {
                put("sdpMid", candidate.sdpMid)
                put("sdpMLineIndex", candidate.sdpMLineIndex)
                put("candidate", candidate.sdp)
            })
        })
    }

    /**
     * Disconnect from the signaling server.
     */
    fun disconnect() {
        Log.d(TAG, "Disconnecting from signaling server")
        webSocket?.close(NORMAL_CLOSURE_STATUS, "Client disconnecting")
        webSocket = null
        isConnected = false
    }

    /**
     * Check if currently connected to the signaling server.
     */
    fun isConnected(): Boolean = isConnected

    /**
     * Handle incoming WebSocket messages.
     */
    private fun handleMessage(text: String) {
        try {
            val json = JSONObject(text)
            val type = json.getString("type")

            Log.d(TAG, "Received message: $type")

            when (type) {
                "room-joined" -> {
                    val roomId = json.getString("roomId")
                    val isInitiator = json.getBoolean("isInitiator")
                    listener.onRoomJoined(roomId, isInitiator)
                }

                "peer-joined" -> {
                    listener.onPeerJoined()
                }

                "peer-left" -> {
                    listener.onPeerLeft()
                }

                "offer" -> {
                    val sdpJson = json.getJSONObject("sdp")
                    val sdp = SessionDescription(
                        SessionDescription.Type.OFFER,
                        sdpJson.getString("sdp")
                    )
                    listener.onOfferReceived(sdp)
                }

                "answer" -> {
                    val sdpJson = json.getJSONObject("sdp")
                    val sdp = SessionDescription(
                        SessionDescription.Type.ANSWER,
                        sdpJson.getString("sdp")
                    )
                    listener.onAnswerReceived(sdp)
                }

                "ice-candidate" -> {
                    val candidateJson = json.getJSONObject("candidate")
                    val candidate = IceCandidate(
                        candidateJson.getString("sdpMid"),
                        candidateJson.getInt("sdpMLineIndex"),
                        candidateJson.getString("candidate")
                    )
                    listener.onIceCandidateReceived(candidate)
                }

                "error" -> {
                    val message = json.optString("message", "Unknown error")
                    listener.onError(message)
                }

                "room-full" -> {
                    listener.onError("Room is full. Only 2 participants allowed.")
                }
            }
        } catch (e: Exception) {
            Log.e(TAG, "Error handling message: ${e.message}")
            listener.onError("Failed to parse signaling message")
        }
    }

    /**
     * Send a JSON message through the WebSocket.
     */
    private fun sendMessage(json: JSONObject) {
        if (!isConnected) {
            Log.w(TAG, "Not connected, cannot send message")
            return
        }

        val message = json.toString()
        Log.d(TAG, "Sending message: ${json.getString("type")}")
        webSocket?.send(message)
    }
}
