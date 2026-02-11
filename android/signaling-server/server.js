/**
 * WebSocket Signaling Server for One-to-One Voice Chat
 *
 * This server facilitates WebRTC signaling between two peers:
 * - Room-based peer matching (two users per room)
 * - SDP offer/answer relay
 * - ICE candidate relay
 *
 * Usage:
 *   node server.js [port]
 *   Default port: 8080
 *
 * Protocol Messages:
 *   Client -> Server:
 *     { type: "join", roomId: "..." }
 *     { type: "leave", roomId: "..." }
 *     { type: "offer", roomId: "...", sdp: {...} }
 *     { type: "answer", roomId: "...", sdp: {...} }
 *     { type: "ice-candidate", roomId: "...", candidate: {...} }
 *
 *   Server -> Client:
 *     { type: "room-joined", roomId: "...", isInitiator: true/false }
 *     { type: "peer-joined" }
 *     { type: "peer-left" }
 *     { type: "offer", sdp: {...} }
 *     { type: "answer", sdp: {...} }
 *     { type: "ice-candidate", candidate: {...} }
 *     { type: "room-full" }
 *     { type: "error", message: "..." }
 */

const WebSocket = require("ws");

const PORT = process.argv[2] || 8080;

// Room storage: roomId -> Set of WebSocket clients
const rooms = new Map();
// Track which room each client is in
const clientRooms = new Map();

const wss = new WebSocket.Server({ port: PORT }, () => {
  console.log(`Signaling server running on ws://0.0.0.0:${PORT}`);
  console.log("Waiting for connections...\n");
});

wss.on("connection", (ws, req) => {
  const clientIp =
    req.headers["x-forwarded-for"] || req.socket.remoteAddress;
  console.log(`Client connected from ${clientIp}`);

  ws.isAlive = true;

  ws.on("pong", () => {
    ws.isAlive = true;
  });

  ws.on("message", (data) => {
    try {
      const message = JSON.parse(data.toString());
      handleMessage(ws, message);
    } catch (error) {
      console.error("Failed to parse message:", error.message);
      sendToClient(ws, { type: "error", message: "Invalid message format" });
    }
  });

  ws.on("close", () => {
    console.log(`Client disconnected from ${clientIp}`);
    handleClientDisconnect(ws);
  });

  ws.on("error", (error) => {
    console.error(`WebSocket error: ${error.message}`);
  });
});

// Heartbeat to detect stale connections
const heartbeat = setInterval(() => {
  wss.clients.forEach((ws) => {
    if (!ws.isAlive) {
      console.log("Terminating stale connection");
      handleClientDisconnect(ws);
      return ws.terminate();
    }
    ws.isAlive = false;
    ws.ping();
  });
}, 30000);

wss.on("close", () => {
  clearInterval(heartbeat);
});

/**
 * Route incoming messages to the appropriate handler.
 */
function handleMessage(ws, message) {
  const { type, roomId } = message;

  switch (type) {
    case "join":
      handleJoinRoom(ws, roomId);
      break;

    case "leave":
      handleLeaveRoom(ws, roomId);
      break;

    case "offer":
    case "answer":
    case "ice-candidate":
      relayMessage(ws, message);
      break;

    default:
      sendToClient(ws, {
        type: "error",
        message: `Unknown message type: ${type}`,
      });
  }
}

/**
 * Handle a client joining a room.
 * Max 2 clients per room for one-to-one chat.
 */
function handleJoinRoom(ws, roomId) {
  if (!roomId) {
    sendToClient(ws, { type: "error", message: "Room ID is required" });
    return;
  }

  // Leave any existing room first
  const currentRoom = clientRooms.get(ws);
  if (currentRoom) {
    handleLeaveRoom(ws, currentRoom);
  }

  // Get or create room
  if (!rooms.has(roomId)) {
    rooms.set(roomId, new Set());
  }

  const room = rooms.get(roomId);

  // Check if room is full (max 2 for 1-to-1)
  if (room.size >= 2) {
    sendToClient(ws, { type: "room-full" });
    console.log(`Room ${roomId} is full, rejecting client`);
    return;
  }

  const isInitiator = room.size === 0;

  // Add client to room
  room.add(ws);
  clientRooms.set(ws, roomId);

  // Notify the joining client
  sendToClient(ws, {
    type: "room-joined",
    roomId: roomId,
    isInitiator: isInitiator,
  });

  console.log(
    `Client joined room "${roomId}" as ${isInitiator ? "initiator" : "peer"} (${room.size}/2)`
  );

  // If this is the second client, notify the first one
  if (!isInitiator) {
    room.forEach((client) => {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        sendToClient(client, { type: "peer-joined" });
      }
    });
  }
}

/**
 * Handle a client leaving a room.
 */
function handleLeaveRoom(ws, roomId) {
  const room = rooms.get(roomId);
  if (!room) return;

  room.delete(ws);
  clientRooms.delete(ws);

  // Notify remaining peer
  room.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      sendToClient(client, { type: "peer-left" });
    }
  });

  // Clean up empty room
  if (room.size === 0) {
    rooms.delete(roomId);
    console.log(`Room "${roomId}" removed (empty)`);
  } else {
    console.log(`Client left room "${roomId}" (${room.size}/2)`);
  }
}

/**
 * Handle client disconnect - clean up room membership.
 */
function handleClientDisconnect(ws) {
  const roomId = clientRooms.get(ws);
  if (roomId) {
    handleLeaveRoom(ws, roomId);
  }
}

/**
 * Relay a signaling message to the other peer in the room.
 */
function relayMessage(ws, message) {
  const roomId = message.roomId;
  const room = rooms.get(roomId);

  if (!room) {
    sendToClient(ws, { type: "error", message: "Room not found" });
    return;
  }

  // Send to all other clients in the room (should be just 1)
  room.forEach((client) => {
    if (client !== ws && client.readyState === WebSocket.OPEN) {
      // Remove roomId from relayed message for cleanliness
      const { roomId: _, ...relayData } = message;
      sendToClient(client, relayData);
    }
  });
}

/**
 * Send a JSON message to a client.
 */
function sendToClient(ws, data) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(JSON.stringify(data));
  }
}

// Log server stats periodically
setInterval(() => {
  const totalClients = wss.clients.size;
  const totalRooms = rooms.size;
  if (totalClients > 0 || totalRooms > 0) {
    console.log(
      `[Stats] Active clients: ${totalClients}, Active rooms: ${totalRooms}`
    );
  }
}, 60000);

// Handle graceful shutdown
process.on("SIGINT", () => {
  console.log("\nShutting down server...");
  wss.close(() => {
    console.log("Server stopped.");
    process.exit(0);
  });
});
