# Voice Chat - Android One-to-One Voice Chat App

A peer-to-peer voice chat Android application built with **WebRTC** for real-time audio communication and **WebSocket** signaling for peer discovery.

## Features

- **One-to-One Voice Chat**: Private voice calls between two users
- **Room-Based Matching**: Users join a shared room ID to connect
- **WebRTC Audio**: Low-latency, high-quality peer-to-peer audio using Google WebRTC
- **NAT Traversal**: STUN servers for connectivity across different networks
- **Audio Controls**: Mute/unmute microphone, toggle speaker/earpiece
- **Call Timer**: Displays elapsed call duration
- **Foreground Service**: Keeps the call alive when the app is in the background
- **Modern UI**: Material Design 3 with a clean dark theme
- **Echo Cancellation**: Hardware-accelerated AEC and noise suppression

## Architecture

```
┌─────────────┐     WebSocket     ┌──────────────────┐     WebSocket     ┌─────────────┐
│  Android     │◄────────────────►│  Signaling Server │◄────────────────►│  Android     │
│  Device A    │                   │  (Node.js)        │                   │  Device B    │
│              │                   └──────────────────┘                   │              │
│              │◄─────────────── WebRTC P2P Audio ──────────────────────►│              │
└─────────────┘                                                          └─────────────┘
```

### Components

| Component | Description |
|-----------|-------------|
| `MainActivity` | Main UI with connect, waiting, and call screens |
| `WebRTCManager` | Manages WebRTC peer connection, audio tracks, and SDP negotiation |
| `SignalingClient` | WebSocket client for exchanging signaling messages |
| `AudioManagerHelper` | Manages Android audio routing (speaker/earpiece) and focus |
| `VoiceChatService` | Foreground service for background call persistence |
| `CallTimer` | Tracks and displays call duration |

### Signaling Flow

1. **User A** connects to the signaling server and joins a room → becomes the **initiator**
2. **User B** connects and joins the same room → server notifies User A
3. **User A** creates a WebRTC offer and sends it via the signaling server
4. **User B** receives the offer, creates an answer, and sends it back
5. Both peers exchange ICE candidates through the signaling server
6. WebRTC establishes a direct peer-to-peer audio connection
7. The signaling server is no longer needed for the actual audio stream

## Prerequisites

- **Android Studio** Arctic Fox (2020.3.1) or later
- **Android SDK** API level 24+ (Android 7.0+)
- **JDK** 8 or later
- **Node.js** 14+ (for the signaling server)

## Setup & Running

### 1. Start the Signaling Server

```bash
cd signaling-server
npm install
npm start
```

The server starts on port 8080 by default. You can specify a custom port:

```bash
node server.js 9090
```

Note your machine's local IP address (e.g., `192.168.1.100`). Both Android devices need to reach this server.

### 2. Build the Android App

Open the `android/` directory in Android Studio:

1. **File → Open** → select the `android/` folder
2. Wait for Gradle sync to complete
3. Connect your Android device or start an emulator
4. Click **Run** (or `Shift+F10`)

### 3. Start a Voice Chat

On **Device A** (or Emulator 1):
1. Enter the signaling server URL: `ws://YOUR_IP:8080`
2. Enter a room ID (e.g., `my-chat-room`)
3. Tap **Join Room**
4. Wait for the peer

On **Device B** (or Emulator 2):
1. Enter the same server URL: `ws://YOUR_IP:8080`
2. Enter the **same** room ID: `my-chat-room`
3. Tap **Join Room**
4. The call connects automatically

### Testing on Emulators

When using Android emulators on the same machine:
- Use `10.0.2.2` instead of `localhost` to reach the host machine
- Server URL: `ws://10.0.2.2:8080`

## Project Structure

```
android/
├── app/
│   ├── build.gradle.kts              # App-level build config
│   ├── proguard-rules.pro            # ProGuard rules
│   └── src/main/
│       ├── AndroidManifest.xml       # App manifest with permissions
│       ├── java/com/voicechat/app/
│       │   ├── MainActivity.kt       # Main UI and call logic
│       │   ├── WebRTCManager.kt      # WebRTC peer connection manager
│       │   ├── SignalingClient.kt    # WebSocket signaling client
│       │   ├── AudioManagerHelper.kt # Audio routing helper
│       │   ├── VoiceChatService.kt   # Foreground service for calls
│       │   └── CallTimer.kt         # Call duration tracker
│       └── res/
│           ├── layout/
│           │   └── activity_main.xml  # Main activity layout
│           ├── drawable/              # Vector icons
│           └── values/
│               ├── colors.xml        # Color palette
│               ├── strings.xml       # String resources
│               └── themes.xml        # Material Design theme
├── signaling-server/
│   ├── package.json                  # Server dependencies
│   └── server.js                     # WebSocket signaling server
├── build.gradle.kts                  # Project-level build config
├── settings.gradle.kts               # Gradle settings
└── README.md                         # This file
```

## Dependencies

### Android App

| Dependency | Purpose |
|-----------|---------|
| `stream-webrtc-android` | WebRTC for Android (peer-to-peer audio) |
| `okhttp3` | WebSocket client for signaling |
| `material` | Material Design 3 components |
| `constraintlayout` | Responsive layouts |
| `kotlinx-coroutines` | Asynchronous operations |

### Signaling Server

| Dependency | Purpose |
|-----------|---------|
| `ws` | WebSocket server implementation |

## Permissions

The app requires the following permissions:

| Permission | Purpose |
|-----------|---------|
| `INTERNET` | Network access for signaling and WebRTC |
| `RECORD_AUDIO` | Microphone access for voice capture |
| `MODIFY_AUDIO_SETTINGS` | Speaker/earpiece routing |
| `ACCESS_NETWORK_STATE` | Network connectivity checks |
| `BLUETOOTH` / `BLUETOOTH_CONNECT` | Bluetooth headset support |
| `FOREGROUND_SERVICE` | Background call persistence |

## Troubleshooting

### Cannot connect to signaling server
- Ensure the server is running and accessible from the device's network
- Check that the IP address is correct (not `localhost` on a real device)
- Verify firewall allows connections on the server port

### No audio after connecting
- Check that microphone permission was granted
- Ensure both devices are on networks that allow peer-to-peer connectivity
- Try using a TURN server if behind strict NATs (corporate firewalls)

### Call drops immediately
- Check the logcat output for WebRTC errors
- Ensure ICE candidates are being exchanged properly
- Verify the signaling server is relaying messages correctly

## License

This project is provided as-is for educational and demonstration purposes.
