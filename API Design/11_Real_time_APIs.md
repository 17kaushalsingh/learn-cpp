# Chapter 11: Real-time APIs

## Introduction to Real-time APIs

Real-time APIs enable instantaneous communication between clients and servers, allowing data to be pushed from server to client as soon as it becomes available, without the client needing to repeatedly poll for updates.

### Why Real-time APIs Matter

- **Instant Updates**: Users see changes immediately without refreshing
- **Better User Experience**: Responsive applications with live data
- **Reduced Server Load**: No need for constant polling requests
- **Modern Applications**: Essential for chat, notifications, live tracking, collaboration

### Types of Real-time Communication

1. **WebSockets**: Full-duplex, persistent connections
2. **Server-Sent Events (SSE)**: One-way server-to-client streaming
3. **Long Polling**: Extended HTTP requests
4. **WebRTC**: Peer-to-peer real-time communication

## WebSockets

### What are WebSockets?

WebSockets provide a persistent, bidirectional communication channel between a client and server over a single TCP connection. Unlike HTTP, which is request-response based, WebSockets allow both client and server to send messages at any time.

### WebSocket Lifecycle

```javascript
// WebSocket Connection Lifecycle
const ws = new WebSocket('ws://localhost:3000');

// 1. Connection Opening
ws.onopen = function(event) {
  console.log('WebSocket connection established');
  console.log('Ready state:', ws.readyState); // 1 (OPEN)

  // Send initial message to server
  ws.send(JSON.stringify({
    type: 'join',
    room: 'general',
    user: 'JohnDoe'
  }));
};

// 2. Message Handling
ws.onmessage = function(event) {
  const message = JSON.parse(event.data);
  console.log('Received message:', message);

  switch(message.type) {
    case 'chat':
      displayChatMessage(message.user, message.text);
      break;
    case 'notification':
      showNotification(message.content);
      break;
    case 'user_joined':
      updateUserList(message.users);
      break;
  }
};

// 3. Error Handling
ws.onerror = function(error) {
  console.error('WebSocket error:', error);
  // Attempt to reconnect
  setTimeout(() => {
    ws.reconnect();
  }, 5000);
};

// 4. Connection Closing
ws.onclose = function(event) {
  console.log('WebSocket connection closed');
  console.log('Code:', event.code);
  console.log('Reason:', event.reason);

  if (event.code !== 1000) { // Not a normal closure
    // Attempt to reconnect
    setTimeout(reconnectWebSocket, 3000);
  }
};

// 5. Manual Close
function closeConnection() {
  ws.close(1000, 'Client disconnecting');
}

// WebSocket Ready States
const READY_STATES = {
  CONNECTING: 0,    // Connection is in progress
  OPEN: 1,          // Connection is established and ready
  CLOSING: 2,       // Connection is closing
  CLOSED: 3         // Connection is closed
};
```

### Server-Side WebSocket Implementation (Node.js)

```javascript
// WebSocket Server Implementation
const WebSocket = require('ws');
const http = require('http');

// Create HTTP server for WebSocket upgrade
const server = http.createServer();
const wss = new WebSocket.Server({ server });

// Store connected clients
const clients = new Map();
const rooms = new Map();

// WebSocket connection handler
wss.on('connection', function connection(ws, req) {
  const clientId = generateClientId();
  const clientInfo = {
    id: clientId,
    ws: ws,
    user: null,
    room: null,
    lastPing: Date.now()
  };

  clients.set(clientId, clientInfo);
  console.log(`Client ${clientId} connected`);

  // Handle incoming messages
  ws.on('message', function incoming(message) {
    try {
      const data = JSON.parse(message);
      handleMessage(clientId, data);
    } catch (error) {
      console.error('Invalid message format:', error);
      sendError(ws, 'Invalid message format');
    }
  });

  // Handle client disconnection
  ws.on('close', function close(code, reason) {
    handleDisconnection(clientId, code, reason);
  });

  // Handle connection errors
  ws.on('error', function error(err) {
    console.error(`Client ${clientId} error:`, err);
  });

  // Send welcome message
  sendMessage(ws, {
    type: 'welcome',
    clientId: clientId
  });
});

// Message handler
function handleMessage(clientId, data) {
  const client = clients.get(clientId);

  switch (data.type) {
    case 'join':
      handleJoinRoom(client, data);
      break;

    case 'leave':
      handleLeaveRoom(client);
      break;

    case 'chat':
      handleChatMessage(client, data);
      break;

    case 'typing':
      handleTypingIndicator(client, data);
      break;

    case 'private_message':
      handlePrivateMessage(client, data);
      break;

    default:
      sendError(client.ws, 'Unknown message type');
  }
}

// Join room handler
function handleJoinRoom(client, data) {
  const { room, user } = data;

  // Leave current room if any
  if (client.room) {
    handleLeaveRoom(client);
  }

  // Join new room
  client.room = room;
  client.user = user;

  // Add to room
  if (!rooms.has(room)) {
    rooms.set(room, new Set());
  }
  rooms.get(room).add(client.id);

  // Get room users
  const roomUsers = Array.from(rooms.get(room))
    .map(id => clients.get(id))
    .filter(c => c.user)
    .map(c => c.user);

  // Send join confirmation
  sendMessage(client.ws, {
    type: 'room_joined',
    room: room,
    users: roomUsers
  });

  // Notify others in room
  broadcastToRoom(room, {
    type: 'user_joined',
    user: user,
    users: roomUsers
  }, client.id);
}

// Chat message handler
function handleChatMessage(client, data) {
  if (!client.room || !client.user) {
    sendError(client.ws, 'Not in a room');
    return;
  }

  const message = {
    type: 'chat',
    id: generateMessageId(),
    user: client.user,
    text: data.text,
    timestamp: new Date().toISOString(),
    room: client.room
  };

  // Broadcast to room
  broadcastToRoom(client.room, message);

  // Store message history (optional)
  storeMessage(message);
}

// Broadcast message to all clients in a room
function broadcastToRoom(room, message, excludeClientId = null) {
  const roomClients = rooms.get(room);
  if (!roomClients) return;

  roomClients.forEach(clientId => {
    if (clientId !== excludeClientId) {
      const client = clients.get(clientId);
      if (client && client.ws.readyState === WebSocket.OPEN) {
        sendMessage(client.ws, message);
      }
    }
  });
}

// Send message to specific client
function sendMessage(ws, data) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(JSON.stringify(data));
  }
}

// Handle client disconnection
function handleDisconnection(clientId, code, reason) {
  const client = clients.get(clientId);
  if (!client) return;

  console.log(`Client ${clientId} disconnected: ${code} - ${reason}`);

  // Remove from room
  if (client.room && client.user) {
    const roomClients = rooms.get(client.room);
    if (roomClients) {
      roomClients.delete(clientId);

      // Notify other users
      broadcastToRoom(client.room, {
        type: 'user_left',
        user: client.user
      });

      // Clean up empty rooms
      if (roomClients.size === 0) {
        rooms.delete(client.room);
      }
    }
  }

  // Remove client
  clients.delete(clientId);
}

// Heartbeat/ping mechanism
setInterval(() => {
  const now = Date.now();
  clients.forEach((client, clientId) => {
    if (now - client.lastPing > 30000) { // 30 seconds timeout
      console.log(`Client ${clientId} timeout, terminating connection`);
      client.ws.terminate();
    }
  });
}, 10000);

// Start server
const PORT = process.env.WS_PORT || 3000;
server.listen(PORT, () => {
  console.log(`WebSocket server running on port ${PORT}`);
});

// Utility functions
function generateClientId() {
  return Math.random().toString(36).substr(2, 9);
}

function generateMessageId() {
  return Date.now().toString(36) + Math.random().toString(36).substr(2);
}

function sendError(ws, message) {
  sendMessage(ws, {
    type: 'error',
    message: message
  });
}

function storeMessage(message) {
  // Store in database or cache for message history
  // This is a placeholder for actual storage implementation
}
```

### Client-Side WebSocket Implementation

```javascript
// Real-time Chat Application
class RealTimeChat {
  constructor() {
    this.ws = null;
    this.room = null;
    this.user = null;
    this.reconnectAttempts = 0;
    this.maxReconnectAttempts = 5;
    this.reconnectDelay = 1000;
  }

  connect(url, user) {
    this.user = user;

    try {
      this.ws = new WebSocket(url);
      this.setupEventHandlers();
    } catch (error) {
      console.error('WebSocket connection error:', error);
      this.handleReconnect();
    }
  }

  setupEventHandlers() {
    this.ws.onopen = () => {
      console.log('Connected to chat server');
      this.reconnectAttempts = 0;
      this.onConnectionChange(true);
    };

    this.ws.onmessage = (event) => {
      const message = JSON.parse(event.data);
      this.handleMessage(message);
    };

    this.ws.onclose = (event) => {
      console.log('Disconnected from chat server');
      this.onConnectionChange(false);

      if (event.code !== 1000) { // Not a normal closure
        this.handleReconnect();
      }
    };

    this.ws.onerror = (error) => {
      console.error('WebSocket error:', error);
      this.onError(error);
    };
  }

  handleMessage(message) {
    switch (message.type) {
      case 'welcome':
        this.onWelcome(message.clientId);
        break;

      case 'room_joined':
        this.room = message.room;
        this.onRoomJoined(message.room, message.users);
        break;

      case 'chat':
        this.onChatMessage(message);
        break;

      case 'user_joined':
        this.onUserJoined(message.user, message.users);
        break;

      case 'user_left':
        this.onUserLeft(message.user);
        break;

      case 'typing':
        this.onTypingIndicator(message.user, message.isTyping);
        break;

      case 'error':
        this.onError(message.message);
        break;
    }
  }

  joinRoom(roomName) {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      this.sendMessage({
        type: 'join',
        room: roomName,
        user: this.user
      });
    }
  }

  leaveRoom() {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      this.sendMessage({
        type: 'leave'
      });
    }
  }

  sendMessage(message) {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      this.ws.send(JSON.stringify(message));
    }
  }

  sendChatMessage(text) {
    this.sendMessage({
      type: 'chat',
      text: text
    });
  }

  sendTypingIndicator(isTyping) {
    this.sendMessage({
      type: 'typing',
      isTyping: isTyping
    });
  }

  sendPrivateMessage(targetUser, text) {
    this.sendMessage({
      type: 'private_message',
      target: targetUser,
      text: text
    });
  }

  handleReconnect() {
    if (this.reconnectAttempts < this.maxReconnectAttempts) {
      this.reconnectAttempts++;
      const delay = this.reconnectDelay * Math.pow(2, this.reconnectAttempts - 1);

      console.log(`Reconnecting... Attempt ${this.reconnectAttempts}/${this.maxReconnectAttempts}`);

      setTimeout(() => {
        this.connect(this.ws.url, this.user);
      }, delay);
    } else {
      console.error('Max reconnection attempts reached');
      this.onMaxReconnectAttemptsReached();
    }
  }

  disconnect() {
    if (this.ws) {
      this.ws.close(1000, 'User disconnecting');
      this.ws = null;
    }
  }

  // Event callbacks (to be implemented by the application)
  onConnectionChange(isConnected) {}
  onWelcome(clientId) {}
  onRoomJoined(room, users) {}
  onChatMessage(message) {}
  onUserJoined(user, users) {}
  onUserLeft(user) {}
  onTypingIndicator(user, isTyping) {}
  onError(error) {}
  onMaxReconnectAttemptsReached() {}
}

// Usage Example
const chat = new RealTimeChat();

chat.onConnectionChange = (isConnected) => {
  const statusEl = document.getElementById('connection-status');
  statusEl.textContent = isConnected ? 'Connected' : 'Disconnected';
  statusEl.className = isConnected ? 'status-connected' : 'status-disconnected';
};

chat.onChatMessage = (message) => {
  displayMessage(message.user, message.text, message.timestamp);
};

chat.onUserJoined = (user, users) => {
  updateUserList(users);
  showNotification(`${user.name} joined the room`);
};

chat.onUserLeft = (user) => {
  showNotification(`${user.name} left the room`);
};

// Connect to chat
const user = {
  id: 'user123',
  name: 'John Doe',
  avatar: 'https://example.com/avatars/john.jpg'
};

chat.connect('ws://localhost:3000', user);
```

## Server-Sent Events (SSE)

### What are Server-Sent Events?

Server-Sent Events provide a one-way communication channel from server to client over standard HTTP. Unlike WebSockets, SSE is simpler to implement and works over standard HTTP infrastructure.

### Server-Side SSE Implementation

```javascript
// SSE Server Implementation
const express = require('express');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

// Store active SSE connections
const sseClients = new Set();

// SSE endpoint
app.get('/events', (req, res) => {
  // Set SSE headers
  res.writeHead(200, {
    'Content-Type': 'text/event-stream',
    'Cache-Control': 'no-cache',
    'Connection': 'keep-alive',
    'Access-Control-Allow-Origin': '*',
    'Access-Control-Allow-Headers': 'Cache-Control'
  });

  // Send initial connection message
  res.write('event: connect\n');
  res.write(`data: ${JSON.stringify({
    type: 'connected',
    timestamp: new Date().toISOString()
  })}\n\n`);

  // Store connection
  const clientId = generateClientId();
  const client = { id: clientId, response: res };
  sseClients.add(client);

  console.log(`SSE client connected: ${clientId}`);

  // Handle client disconnect
  req.on('close', () => {
    console.log(`SSE client disconnected: ${clientId}`);
    sseClients.delete(client);
  });

  // Send periodic ping to keep connection alive
  const pingInterval = setInterval(() => {
    if (res.writable) {
      res.write('event: ping\n');
      res.write(`data: ${JSON.stringify({
        timestamp: new Date().toISOString()
      })}\n\n`);
    } else {
      clearInterval(pingInterval);
      sseClients.delete(client);
    }
  }, 30000); // Every 30 seconds

  // Cleanup on connection close
  req.on('close', () => {
    clearInterval(pingInterval);
  });
});

// Broadcast event to all SSE clients
function broadcastSSE(event, data) {
  const message = JSON.stringify(data);

  sseClients.forEach(client => {
    if (client.response.writable) {
      client.response.write(`event: ${event}\n`);
      client.response.write(`data: ${message}\n\n`);
    } else {
      // Remove dead connection
      sseClients.delete(client);
    }
  });
}

// API endpoints that trigger SSE events
app.post('/notifications', (req, res) => {
  const { userId, message, type } = req.body;

  const notification = {
    id: generateNotificationId(),
    userId,
    message,
    type,
    timestamp: new Date().toISOString()
  };

  // Broadcast to specific user or all users
  if (userId) {
    broadcastToUser(userId, 'notification', notification);
  } else {
    broadcastSSE('notification', notification);
  }

  res.json({ success: true, notificationId: notification.id });
});

app.post('/updates', (req, res) => {
  const { resource, action, data } = req.body;

  const update = {
    resource,
    action,
    data,
    timestamp: new Date().toISOString()
  };

  broadcastSSE('update', update);
  res.json({ success: true });
});

// Real-time stock prices example
const stockPrices = {
  AAPL: 150.25,
  GOOGL: 2800.50,
  MSFT: 300.75,
  TSLA: 800.00
};

// Simulate stock price updates
setInterval(() => {
  const symbols = Object.keys(stockPrices);
  const randomSymbol = symbols[Math.floor(Math.random() * symbols.length)];

  // Random price change between -2% and +2%
  const changePercent = (Math.random() - 0.5) * 0.04;
  stockPrices[randomSymbol] *= (1 + changePercent);

  const priceUpdate = {
    symbol: randomSymbol,
    price: stockPrices[randomSymbol],
    change: changePercent,
    timestamp: new Date().toISOString()
  };

  broadcastSSE('stock_update', priceUpdate);
}, 2000); // Update every 2 seconds

// Live news feed
const newsUpdates = [
  "Breaking: Major tech announcement today",
  "Stock market reaches new high",
  "New product launch creates excitement",
  "Economic indicators show positive growth"
];

let newsIndex = 0;
setInterval(() => {
  const newsItem = {
    id: generateNewsId(),
    headline: newsUpdates[newsIndex],
    timestamp: new Date().toISOString()
  };

  broadcastSSE('news', newsItem);
  newsIndex = (newsIndex + 1) % newsUpdates.length;
}, 30000); // Every 30 seconds

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
  console.log(`SSE server running on port ${PORT}`);
});

function generateClientId() {
  return Math.random().toString(36).substr(2, 9);
}

function generateNotificationId() {
  return Date.now().toString(36);
}

function generateNewsId() {
  return 'news_' + Date.now().toString(36);
}

function broadcastToUser(userId, event, data) {
  const message = JSON.stringify({ ...data, userId });

  sseClients.forEach(client => {
    // In a real implementation, you'd track which client belongs to which user
    // For this example, we broadcast to all clients
    if (client.response.writable) {
      client.response.write(`event: ${event}\n`);
      client.response.write(`data: ${message}\n\n`);
    }
  });
}
```

### Client-Side SSE Implementation

```javascript
// SSE Client Implementation
class EventStreamClient {
  constructor(url) {
    this.url = url;
    this.eventSource = null;
    this.reconnectAttempts = 0;
    this.maxReconnectAttempts = 10;
    this.reconnectDelay = 1000;
    this.isConnected = false;
  }

  connect() {
    try {
      this.eventSource = new EventSource(this.url);
      this.setupEventHandlers();
    } catch (error) {
      console.error('SSE connection error:', error);
      this.handleReconnect();
    }
  }

  setupEventHandlers() {
    // Connection opened
    this.eventSource.onopen = () => {
      console.log('SSE connection established');
      this.isConnected = true;
      this.reconnectAttempts = 0;
      this.onConnectionChange(true);
    };

    // Connection closed
    this.eventSource.onclose = () => {
      console.log('SSE connection closed');
      this.isConnected = false;
      this.onConnectionChange(false);
      this.handleReconnect();
    };

    // Connection error
    this.eventSource.onerror = (error) => {
      console.error('SSE connection error:', error);
      this.isConnected = false;
      this.onConnectionChange(false);

      if (this.eventSource.readyState === EventSource.CLOSED) {
        this.handleReconnect();
      }
    };

    // Generic message handler
    this.eventSource.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        this.onMessage(data);
      } catch (error) {
        console.error('Error parsing SSE message:', error);
      }
    };

    // Specific event type handlers
    this.eventSource.addEventListener('notification', (event) => {
      const notification = JSON.parse(event.data);
      this.onNotification(notification);
    });

    this.eventSource.addEventListener('update', (event) => {
      const update = JSON.parse(event.data);
      this.onUpdate(update);
    });

    this.eventSource.addEventListener('stock_update', (event) => {
      const stockUpdate = JSON.parse(event.data);
      this.onStockUpdate(stockUpdate);
    });

    this.eventSource.addEventListener('news', (event) => {
      const news = JSON.parse(event.data);
      this.onNews(news);
    });

    this.eventSource.addEventListener('ping', (event) => {
      // Handle ping to keep connection alive
      console.log('Ping received');
    });

    this.eventSource.addEventListener('connect', (event) => {
      const data = JSON.parse(event.data);
      this.onConnect(data);
    });
  }

  handleReconnect() {
    if (this.reconnectAttempts < this.maxReconnectAttempts) {
      this.reconnectAttempts++;
      const delay = Math.min(
        this.reconnectDelay * Math.pow(2, this.reconnectAttempts - 1),
        30000 // Max 30 seconds
      );

      console.log(`SSE reconnecting... Attempt ${this.reconnectAttempts}/${this.maxReconnectAttempts}`);

      setTimeout(() => {
        this.connect();
      }, delay);
    } else {
      console.error('Max SSE reconnection attempts reached');
      this.onMaxReconnectAttemptsReached();
    }
  }

  disconnect() {
    if (this.eventSource) {
      this.eventSource.close();
      this.eventSource = null;
      this.isConnected = false;
    }
  }

  // Event callbacks (to be implemented by the application)
  onConnectionChange(isConnected) {}
  onMessage(data) {}
  onConnect(data) {}
  onNotification(notification) {}
  onUpdate(update) {}
  onStockUpdate(stockUpdate) {}
  onNews(news) {}
  onMaxReconnectAttemptsReached() {}
}

// Real-time Dashboard Example
class RealTimeDashboard {
  constructor() {
    this.sseClient = new EventStreamClient('/events');
    this.setupEventHandlers();
    this.sseClient.connect();
  }

  setupEventHandlers() {
    this.sseClient.onConnectionChange = (isConnected) => {
      this.updateConnectionStatus(isConnected);
    };

    this.sseClient.onNotification = (notification) => {
      this.showNotification(notification);
    };

    this.sseClient.onStockUpdate = (stockUpdate) => {
      this.updateStockPrice(stockUpdate);
    };

    this.sseClient.onNews = (news) => {
      this.addNewsItem(news);
    };

    this.sseClient.onUpdate = (update) => {
      this.handleDataUpdate(update);
    };
  }

  updateConnectionStatus(isConnected) {
    const statusEl = document.getElementById('sse-status');
    statusEl.textContent = isConnected ? 'Connected' : 'Disconnected';
    statusEl.className = isConnected ? 'status-connected' : 'status-disconnected';
  }

  showNotification(notification) {
    // Show toast notification
    const toast = document.createElement('div');
    toast.className = `notification notification-${notification.type}`;
    toast.textContent = notification.message;

    document.getElementById('notifications').appendChild(toast);

    // Auto-remove after 5 seconds
    setTimeout(() => {
      toast.remove();
    }, 5000);
  }

  updateStockPrice(stockUpdate) {
    const priceEl = document.getElementById(`stock-${stockUpdate.symbol}`);
    if (priceEl) {
      const currentPrice = parseFloat(priceEl.textContent);
      const changeElement = priceEl.nextElementSibling;

      priceEl.textContent = stockUpdate.price.toFixed(2);

      // Update change indicator
      if (stockUpdate.change > 0) {
        changeElement.textContent = `↑ ${stockUpdate.change.toFixed(2)}%`;
        changeElement.className = 'price-change positive';
      } else {
        changeElement.textContent = `↓ ${Math.abs(stockUpdate.change).toFixed(2)}%`;
        changeElement.className = 'price-change negative';
      }

      // Add animation
      priceEl.classList.add('price-updated');
      setTimeout(() => {
        priceEl.classList.remove('price-updated');
      }, 500);
    }
  }

  addNewsItem(news) {
    const newsContainer = document.getElementById('news-feed');
    const newsItem = document.createElement('div');
    newsItem.className = 'news-item';
    newsItem.innerHTML = `
      <span class="news-time">${new Date(news.timestamp).toLocaleTimeString()}</span>
      <span class="news-headline">${news.headline}</span>
    `;

    newsContainer.insertBefore(newsItem, newsContainer.firstChild);

    // Keep only latest 10 items
    while (newsContainer.children.length > 10) {
      newsContainer.removeChild(newsContainer.lastChild);
    }
  }

  handleDataUpdate(update) {
    switch (update.resource) {
      case 'user':
        this.updateUserData(update.data);
        break;
      case 'order':
        this.updateOrderData(update.data);
        break;
      case 'system':
        this.updateSystemStatus(update.data);
        break;
    }
  }
}

// Initialize dashboard
const dashboard = new RealTimeDashboard();
```

## WebSocket vs SSE Comparison

```javascript
// Comparison Chart
const comparison = {
  websockets: {
    connectionType: 'Bidirectional (Full-duplex)',
    protocol: 'WebSocket Protocol (ws://, wss://)',
    complexity: 'Higher - requires connection management',
    scalability: 'More complex scaling challenges',
    browserSupport: 'Modern browsers, requires polyfills for older',
    infrastructure: 'May require special server configuration',
    useCases: [
      'Chat applications',
      'Real-time gaming',
      'Collaborative editing',
      'Live trading platforms'
    ]
  },

  sse: {
    connectionType: 'Unidirectional (Server to client)',
    protocol: 'Standard HTTP',
    complexity: 'Lower - uses existing HTTP infrastructure',
    scalability: 'Easier to scale with existing web infrastructure',
    browserSupport: 'Excellent, native support in all modern browsers',
    infrastructure: 'Works with existing HTTP servers and proxies',
    useCases: [
      'Live notifications',
      'News feeds',
      'Stock tickers',
      'Status updates',
      'Progress indicators'
    ]
  }
};

// Decision Matrix
function chooseRealtimeTechnology(requirements) {
  if (requirements.bidirectionalCommunication &&
      requirements.lowLatency &&
      requirements.clientToServerMessages) {
    return 'WebSockets';
  }

  if (requirements.serverToClientOnly &&
      requirements.simplicity &&
      requirements.infrastructureCompatibility) {
    return 'Server-Sent Events';
  }

  if (requirements.maximumBrowserCompatibility &&
      requirements.serverToClientOnly) {
    return 'Server-Sent Events';
  }

  return 'WebSockets'; // Default for complex real-time needs
}
```

## Real-time API Design Patterns

### 1. Event-Driven Architecture

```javascript
// Event-driven real-time system
class EventBus {
  constructor() {
    this.events = new Map();
    this.subscribers = new Map();
  }

  // Subscribe to events
  subscribe(event, callback) {
    if (!this.subscribers.has(event)) {
      this.subscribers.set(event, new Set());
    }
    this.subscribers.get(event).add(callback);

    return () => this.unsubscribe(event, callback);
  }

  // Unsubscribe from events
  unsubscribe(event, callback) {
    if (this.subscribers.has(event)) {
      this.subscribers.get(event).delete(callback);
    }
  }

  // Emit event to all subscribers
  emit(event, data) {
    if (this.subscribers.has(event)) {
      this.subscribers.get(event).forEach(callback => {
        try {
          callback(data);
        } catch (error) {
          console.error(`Error in event handler for ${event}:`, error);
        }
      });
    }
  }

  // Store event history
  storeEvent(event, data) {
    if (!this.events.has(event)) {
      this.events.set(event, []);
    }

    const eventData = {
      id: generateEventId(),
      event,
      data,
      timestamp: new Date().toISOString()
    };

    this.events.get(event).push(eventData);

    // Keep only last 100 events per type
    if (this.events.get(event).length > 100) {
      this.events.get(event).shift();
    }

    return eventData;
  }

  // Get event history
  getEventHistory(event, limit = 50) {
    if (this.events.has(event)) {
      return this.events.get(event).slice(-limit);
    }
    return [];
  }
}

// Real-time collaboration example
class CollaborativeEditor {
  constructor(documentId) {
    this.documentId = documentId;
    this.eventBus = new EventBus();
    this.document = {
      id: documentId,
      content: '',
      version: 0,
      lastModified: new Date()
    };
    this.participants = new Map();

    this.setupEventHandlers();
  }

  setupEventHandlers() {
    // Handle text changes
    this.eventBus.subscribe('text_change', (change) => {
      this.applyTextChange(change);
    });

    // Handle cursor movements
    this.eventBus.subscribe('cursor_move', (cursor) => {
      this.updateParticipantCursor(cursor);
    });

    // Handle user joining/leaving
    this.eventBus.subscribe('user_join', (user) => {
      this.addParticipant(user);
    });

    this.eventBus.subscribe('user_leave', (userId) => {
      this.removeParticipant(userId);
    });
  }

  applyTextChange(change) {
    // Apply operational transformation
    const transformedChange = this.transformChange(change);

    // Apply change to document
    this.document.content = this.document.content.slice(0, transformedChange.position) +
                          transformedChange.text +
                          this.document.content.slice(transformedChange.position + transformedChange.length);

    this.document.version++;
    this.document.lastModified = new Date();

    // Broadcast to all participants
    this.broadcastToParticipants('text_change_applied', {
      documentId: this.documentId,
      change: transformedChange,
      version: this.document.version,
      author: change.userId
    });
  }

  transformChange(change) {
    // Simplified operational transformation
    // In real implementation, this would handle concurrent edits
    return {
      position: change.position,
      length: change.length,
      text: change.text,
      userId: change.userId,
      timestamp: change.timestamp
    };
  }

  addParticipant(user) {
    this.participants.set(user.id, {
      ...user,
      cursor: { line: 1, column: 1 },
      lastSeen: new Date()
    });

    this.broadcastToParticipants('participant_joined', {
      user: user,
      participants: Array.from(this.participants.values())
    });
  }

  broadcastToParticipants(event, data) {
    // Send via WebSocket or SSE to all connected participants
    // This would integrate with your WebSocket/SSE server
    console.log(`Broadcasting ${event}:`, data);
  }
}
```

### 2. Live Data Streaming

```javascript
// Live data streaming implementation
class DataStreamer {
  constructor() {
    this.streams = new Map();
    this.subscribers = new Map();
  }

  // Create a data stream
  createStream(streamId, config) {
    const stream = {
      id: streamId,
      config,
      data: [],
      isActive: false,
      interval: null
    };

    this.streams.set(streamId, stream);
    return stream;
  }

  // Start streaming data
  startStream(streamId, dataGenerator, interval = 1000) {
    const stream = this.streams.get(streamId);
    if (!stream) {
      throw new Error(`Stream ${streamId} not found`);
    }

    stream.isActive = true;
    stream.interval = setInterval(() => {
      if (stream.isActive) {
        const data = dataGenerator();
        this.addToStream(streamId, data);
      }
    }, interval);
  }

  // Add data to stream
  addToStream(streamId, data) {
    const stream = this.streams.get(streamId);
    if (!stream) return;

    const dataPoint = {
      id: generateDataId(),
      value: data,
      timestamp: new Date().toISOString()
    };

    stream.data.push(dataPoint);

    // Keep only recent data points
    if (stream.config.maxDataPoints) {
      if (stream.data.length > stream.config.maxDataPoints) {
        stream.data.shift();
      }
    }

    // Notify subscribers
    this.notifySubscribers(streamId, dataPoint);
  }

  // Subscribe to stream
  subscribe(streamId, callback) {
    if (!this.subscribers.has(streamId)) {
      this.subscribers.set(streamId, new Set());
    }

    this.subscribers.get(streamId).add(callback);

    // Send existing data to new subscriber
    const stream = this.streams.get(streamId);
    if (stream && stream.data.length > 0) {
      callback(stream.data);
    }

    return () => this.unsubscribe(streamId, callback);
  }

  // Notify all subscribers
  notifySubscribers(streamId, data) {
    const subscribers = this.subscribers.get(streamId);
    if (subscribers) {
      subscribers.forEach(callback => {
        try {
          callback([data]);
        } catch (error) {
          console.error('Error notifying subscriber:', error);
        }
      });
    }
  }

  // Stop streaming
  stopStream(streamId) {
    const stream = this.streams.get(streamId);
    if (stream) {
      stream.isActive = false;
      if (stream.interval) {
        clearInterval(stream.interval);
        stream.interval = null;
      }
    }
  }
}

// Real-time analytics dashboard
class AnalyticsDashboard {
  constructor() {
    this.dataStreamer = new DataStreamer();
    this.setupStreams();
    this.setupUI();
  }

  setupStreams() {
    // Website visitors stream
    this.dataStreamer.createStream('visitors', {
      maxDataPoints: 100,
      aggregation: 'count'
    });

    // Sales stream
    this.dataStreamer.createStream('sales', {
      maxDataPoints: 50,
      aggregation: 'sum'
    });

    // Performance metrics stream
    this.dataStreamer.createStream('performance', {
      maxDataPoints: 200,
      aggregation: 'average'
    });

    // Start data generation
    this.startDataGeneration();
  }

  startDataGeneration() {
    // Simulate visitor tracking
    this.dataStreamer.startStream('visitors', () => {
      return Math.floor(Math.random() * 50) + 10;
    }, 2000);

    // Simulate sales data
    this.dataStreamer.startStream('sales', () => {
      return Math.random() * 1000 + 100;
    }, 5000);

    // Simulate performance metrics
    this.dataStreamer.startStream('performance', () => {
      return {
        responseTime: Math.random() * 200 + 50,
        throughput: Math.random() * 1000 + 500,
        errorRate: Math.random() * 5
      };
    }, 1000);
  }

  setupUI() {
    // Subscribe to visitors stream
    this.dataStreamer.subscribe('visitors', (data) => {
      this.updateVisitorChart(data);
      this.updateVisitorCount(data[data.length - 1]?.value || 0);
    });

    // Subscribe to sales stream
    this.dataStreamer.subscribe('sales', (data) => {
      this.updateSalesChart(data);
      this.updateTotalSales(data);
    });

    // Subscribe to performance stream
    this.dataStreamer.subscribe('performance', (data) => {
      this.updatePerformanceMetrics(data[data.length - 1]?.value || {});
    });
  }

  updateVisitorChart(data) {
    // Update visitor count chart
    const chart = document.getElementById('visitor-chart');
    if (chart) {
      // Update chart with new data points
      this.updateChart(chart, data.map(d => ({
        x: new Date(d.timestamp),
        y: d.value
      })));
    }
  }

  updateVisitorCount(count) {
    const countEl = document.getElementById('current-visitors');
    if (countEl) {
      countEl.textContent = count;
      countEl.classList.add('updated');
      setTimeout(() => countEl.classList.remove('updated'), 500);
    }
  }

  updateSalesChart(data) {
    const chart = document.getElementById('sales-chart');
    if (chart) {
      this.updateChart(chart, data.map(d => ({
        x: new Date(d.timestamp),
        y: d.value
      })));
    }
  }

  updatePerformanceMetrics(metrics) {
    if (metrics.responseTime) {
      document.getElementById('response-time').textContent =
        `${metrics.responseTime.toFixed(0)}ms`;
    }
    if (metrics.throughput) {
      document.getElementById('throughput').textContent =
        `${metrics.throughput.toFixed(0)} req/s`;
    }
    if (metrics.errorRate !== undefined) {
      document.getElementById('error-rate').textContent =
        `${metrics.errorRate.toFixed(1)}%`;
    }
  }

  updateChart(chartElement, data) {
    // Implementation would depend on charting library (Chart.js, D3.js, etc.)
    console.log('Updating chart with data:', data);
  }
}
```

## Best Practices for Real-time APIs

### 1. Connection Management

```javascript
// Robust connection management
class ConnectionManager {
  constructor() {
    this.connections = new Map();
    this.maxConnections = 10000;
    this.connectionTimeout = 300000; // 5 minutes
    this.heartbeatInterval = 30000; // 30 seconds
  }

  addConnection(id, connection) {
    if (this.connections.size >= this.maxConnections) {
      throw new Error('Maximum connections reached');
    }

    const connectionInfo = {
      id,
      connection,
      lastActivity: Date.now(),
      heartbeatTimer: this.startHeartbeat(id)
    };

    this.connections.set(id, connectionInfo);

    // Setup connection cleanup
    connection.on('close', () => this.removeConnection(id));
    connection.on('error', () => this.removeConnection(id));
  }

  removeConnection(id) {
    const connectionInfo = this.connections.get(id);
    if (connectionInfo) {
      if (connectionInfo.heartbeatTimer) {
        clearInterval(connectionInfo.heartbeatTimer);
      }
      this.connections.delete(id);
    }
  }

  startHeartbeat(id) {
    return setInterval(() => {
      const connectionInfo = this.connections.get(id);
      if (connectionInfo) {
        if (Date.now() - connectionInfo.lastActivity > this.connectionTimeout) {
          console.log(`Connection ${id} timed out`);
          connectionInfo.connection.terminate();
          this.removeConnection(id);
        } else {
          // Send ping
          this.sendPing(connectionInfo.connection);
        }
      }
    }, this.heartbeatInterval);
  }

  updateActivity(id) {
    const connectionInfo = this.connections.get(id);
    if (connectionInfo) {
      connectionInfo.lastActivity = Date.now();
    }
  }

  sendPing(connection) {
    if (connection.readyState === WebSocket.OPEN) {
      connection.ping();
    }
  }
}
```

### 2. Message Queue Integration

```javascript
// Real-time API with message queue
class RealTimeAPI {
  constructor() {
    this.connections = new Map();
    this.messageQueue = new MessageQueue();
    this.eventProcessor = new EventProcessor();

    this.setupMessageQueueHandlers();
  }

  setupMessageQueueHandlers() {
    // Handle messages from other services
    this.messageQueue.subscribe('user.notifications', (message) => {
      this.broadcastToUser(message.userId, 'notification', message.data);
    });

    this.messageQueue.subscribe('system.events', (message) => {
      this.broadcastToAll('system_event', message.data);
    });

    this.messageQueue.subscribe('room.messages', (message) => {
      this.broadcastToRoom(message.roomId, 'message', message.data);
    });
  }

  broadcastToUser(userId, event, data) {
    const userConnections = this.getUserConnections(userId);
    userConnections.forEach(connection => {
      this.sendToConnection(connection, event, data);
    });
  }

  broadcastToRoom(roomId, event, data) {
    const roomConnections = this.getRoomConnections(roomId);
    roomConnections.forEach(connection => {
      this.sendToConnection(connection, event, data);
    });
  }

  broadcastToAll(event, data) {
    this.connections.forEach(connection => {
      this.sendToConnection(connection, event, data);
    });
  }
}
```

### 3. Scaling Real-time APIs

```javascript
// Horizontal scaling with Redis adapter
class RedisAdapter {
  constructor() {
    this.redis = require('redis');
    this.publisher = this.redis.createClient();
    this.subscriber = this.redis.createClient();

    this.setupRedisHandlers();
  }

  setupRedisHandlers() {
    this.subscriber.subscribe('realtime:*');

    this.subscriber.on('message', (channel, message) => {
      const [, event] = channel.split(':');
      this.handleRedisMessage(event, message);
    });
  }

  publish(channel, data) {
    this.publisher.publish(channel, JSON.stringify(data));
  }

  handleRedisMessage(event, message) {
    try {
      const data = JSON.parse(message);
      this.broadcastToLocalConnections(event, data);
    } catch (error) {
      console.error('Error parsing Redis message:', error);
    }
  }
}

// Load balancer for WebSocket connections
class WebSocketLoadBalancer {
  constructor() {
    this.servers = [
      { host: 'ws1.example.com', port: 3001, connections: 0 },
      { host: 'ws2.example.com', port: 3001, connections: 0 },
      { host: 'ws3.example.com', port: 3001, connections: 0 }
    ];
  }

  selectServer() {
    // Least connections algorithm
    return this.servers.reduce((min, current) =>
      current.connections < min.connections ? current : min
    );
  }

  getServerUrl() {
    const server = this.selectServer();
    server.connections++;
    return `ws://${server.host}:${server.port}`;
  }
}
```

## Security Considerations

### 1. Authentication & Authorization

```javascript
// WebSocket authentication middleware
function authenticateWebSocket(socket, next) {
  const token = socket.handshake.auth.token ||
                socket.handshake.headers.authorization?.replace('Bearer ', '');

  if (!token) {
    return next(new Error('Authentication required'));
  }

  try {
    const decoded = jwt.verify(token, process.env.JWT_SECRET);
    socket.user = decoded;
    next();
  } catch (error) {
    next(new Error('Invalid authentication token'));
  }
}

// Rate limiting for WebSocket connections
const wsRateLimiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutes
  max: 100, // 100 connections per window
  message: 'Too many connection attempts'
});

// Message validation
function validateMessage(message) {
  const schema = {
    type: 'object',
    required: ['type'],
    properties: {
      type: { type: 'string' },
      data: { type: 'object' },
      timestamp: { type: 'string', format: 'date-time' }
    }
  };

  return validate(message, schema);
}
```

### 2. Input Sanitization

```javascript
// Sanitize incoming messages
function sanitizeMessage(message) {
  if (typeof message.text === 'string') {
    message.text = sanitizeHtml(message.text, {
      allowedTags: ['b', 'i', 'em', 'strong'],
      allowedAttributes: {}
    });
  }
  return message;
}

// Message size limits
const MAX_MESSAGE_SIZE = 64 * 1024; // 64KB

function validateMessageSize(message) {
  const size = JSON.stringify(message).length;
  if (size > MAX_MESSAGE_SIZE) {
    throw new Error('Message too large');
  }
}
```

## Interview Questions

### Basic Questions

1. **What's the difference between WebSockets and Server-Sent Events?**
   - WebSockets: Bidirectional communication, more complex
   - SSE: Unidirectional (server to client), simpler, uses HTTP

2. **When would you use WebSockets vs HTTP polling?**
   - WebSockets: Real-time, low-latency, frequent updates
   - HTTP polling: Infrequent updates, simpler implementation

3. **What are the main challenges with real-time APIs?**
   - Connection management, scaling, security, reliability
   - State management, message ordering, error handling

### Intermediate Questions

4. **How do you handle WebSocket authentication?**
   - Token-based authentication during connection handshake
   - Verify JWT tokens or session IDs
   - Implement authorization checks for message types

5. **How do you scale real-time applications?**
   - Load balancing across multiple servers
   - Message brokers (Redis, Kafka) for cross-server communication
   - Database optimization and connection pooling

6. **What is CORS and how does it affect WebSockets?**
   - CORS doesn't apply to WebSocket connections
   - But WebSocket handshake is HTTP and subject to CORS rules
   - Modern browsers handle CORS for WebSocket upgrades automatically

### Advanced Questions

7. **How do you ensure message ordering in distributed real-time systems?**
   - Sequence numbers, timestamps, message IDs
   - Use message queues with ordering guarantees
   - Implement compensation for out-of-order delivery

8. **What strategies do you use for handling connection failures?**
   - Exponential backoff reconnection
   - Buffer messages while disconnected
   - Heartbeat/ping mechanisms for connection health

9. **How do you implement real-time features in a microservices architecture?**
   - Event-driven architecture with message brokers
   - API Gateways with WebSocket support
   - Service mesh for inter-service communication

### Scenario-Based Questions

10. **How would you implement a real-time notification system?**
    - SSE for simple notifications
    - WebSockets for interactive notifications
    - Message queues for reliable delivery
    - User preference management for notification types

11. **How would you handle real-time collaboration features like Google Docs?**
    - Operational transformation algorithms
    - Conflict resolution for concurrent edits
    - Efficient synchronization strategies
    - User presence and cursor tracking

12. **How would you monitor and debug a real-time production system?**
    - Connection metrics, message throughput, latency monitoring
    - Distributed tracing for message flows
    - Error tracking and alerting
    - Performance profiling and optimization

## Summary

Real-time APIs enable modern interactive applications by providing instantaneous communication between clients and servers. Key concepts include:

- **WebSockets**: Full-duplex, persistent connections for bidirectional communication
- **Server-Sent Events**: Simple, one-way server-to-client streaming over HTTP
- **Connection Management**: Proper handling of connections, timeouts, and reconnections
- **Scaling Strategies**: Load balancing, message queues, and distributed architectures
- **Security**: Authentication, authorization, and input validation for real-time connections

Success factors:
- Choose the right technology based on requirements
- Implement robust connection management and error handling
- Design for scalability from the beginning
- Monitor performance and user experience
- Plan for security and reliability challenges

Real-time APIs are essential for modern applications requiring instant updates, collaborative features, or live data streaming. Proper implementation ensures responsive, reliable, and scalable user experiences.