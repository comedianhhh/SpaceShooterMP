# Space Shooter MP 🚀💥  
*A C++ multiplayer space shooter built with a custom game engine, supporting client-server architecture.*

[![C++](https://img.shields.io/badge/C%2B%2B-17-%2300599C?logo=c%2B%2B)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)
[![Network](https://img.shields.io/badge/Network-Asio-%2300A8E1)](https://think-async.com/Asio/)

**Gameplay Preview**  
<!-- Add multiplayer gameplay GIF/screenshot here -->
![Multiplayer Demo](media/demo.gif)

---

## 🌌 Key Features
- **Multiplayer Architecture**
  - Client-server model with multiple client support
  - Synchronized game state across players
  - Player position and bullet prediction
- **Custom Engine Components**
  - Entity-component system (ECS) architecture
  - Asteroid spawning system with wave management
  - Collision detection and resolution
- **Network Features**
  - Low-latency UDP communication
  - Client-side prediction
  - Server reconciliation

---

## 🛠️ Technical Implementation

### Core Classes
| Class | Responsibility |
|-------|----------------|
| `GameHandler` | Manages network connections and game state |
| `AsteroidSpawner` | Controls asteroid generation patterns |
| `Player` | Handles player movement and shooting |
| `Bullet` | Manages projectile physics and collisions |

### Network Architecture
```cpp
// GameHandler.h - Multi-client management
class GameHandler {
public:
    void HandleClientConnection(Client& client);
    void BroadcastGameState();
    void UpdatePlayerPosition(int clientId, Vector2D newPos);
    
private:
    std::vector<Client> connectedClients;
    GameState currentState;
};
```
### Game Loop Snippet
```cpp

// main.cpp - Engine core
while (gameRunning) {
    double deltaTime = CalculateDeltaTime();
    
    // Process network messages
    gameHandler.ProcessNetworkEvents();
    
    // Update game state
    playerSystem.Update(deltaTime);
    collisionSystem.DetectCollisions();
    
    // Render frame
    renderSystem.Draw();
}
```
### 🎮 Controls
Action	Control
Move	W/A/S/D
Shoot	Spacebar
Rotate	Mouse Movement
Pause	ESC
### 📡 Networking Overview
Protocol Structure
Component	Format
Player Position	{ID,X,Y,ROTATION}
Bullet Spawn	{TYPE,X,Y,DIRECTION}
Game State	JSON serialized object


