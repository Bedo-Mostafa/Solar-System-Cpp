# 🌌 Polished Solar System Simulation

A high-fidelity, physics-based space simulation engine built with **C++17**, **SFML**, and **Box2D**.

This project simulates Newtonian gravity, orbital mechanics, and celestial collisions using a modular architecture based on **SOLID principles**. It features procedural generation, custom GLSL shaders for 3D volumetric effects, and a self-healing build system.

<p align="center">
  <img src="assets/Demo.gif" alt="Solar System Demo" width="100%">
</p>

---

## ✨ Key Features

### 🪐 Physics & Simulation
* **Newtonian Gravity Engine:** Real-time calculation of $F = G \frac{m_1 m_2}{r^2}$ for realistic orbital mechanics.
* **Automatic Orbit Injection:** New planets automatically calculate the precise tangential velocity required for a stable circular orbit relative to the Sun.
* **Collision Event System:**
    * **Sun Destruction:** Planets that hit the Sun are pulled in via a "Black Hole" suction effect.
    * **Planet Merging:** Planets colliding with each other are destroyed/absorbed rather than bouncing unrealistically.

### 🎨 Cinematic Visuals (GLSL Shaders)
* **Milky Way Background:** A procedurally generated fragment shader creating a dynamic, moving nebula and twinkling starfield.
* **3D Planet Rendering:** Custom lighting shaders turn 2D circles into 3D-looking spheres with rim lighting and shadows based on the Sun's position.
* **Glowing Sun:** Radial gradient shader creating a bright core and soft corona.
* **Orbital Trails:** Fading vertex arrays that track planetary paths, smoothing out movement.
* **Cinematic Intro:** A smooth, interpolated camera zoom-out animation upon application start.

### 📊 User Interface
* **Live Dashboard:** Displays entity count, FPS, and the governing physics equations.
* **Dynamic Labels:** Every planet displays its Mass and Velocity in real-time, floating above the object.

### 🎵 Audio
* **Ambient Soundtrack:** Integrated background music support (WAV/OGG).

---

## 📐 System Design (UML & SOLID)

The project follows a strict Object-Oriented design adhering to **SOLID principles** (Single Responsibility, Open/Closed, Liskov Substitution, Dependency Inversion).

### Class Diagram

```mermaid
classDiagram
    class Game {
        -RenderWindow window
        -View camera
        +run()
        -update(dt)
        -render()
    }

    class CelestialBody {
        <<Abstract>>
        #b2Body* body
        #Shape shape
        +render(window, shader)
        +update(dt)
        +getMass()
        +getPosition()
    }

    class Sun {
        -Shader shader
        +render()
    }

    class Planet {
        -State state
        -VertexArray trail
        -Text label
        +update(dt)
        +startDeath()
    }

    class PhysicsWorld {
        -b2World world
        -ContactListener listener
        +update()
    }

    class GravitySystem {
        <<Static>>
        +applyGravity(Sun, Planet)
        +calculateOrbitVelocity()
    }

    class GameResources {
        +Shader sunShader
        +Shader planetShader
        +Font font
        +Music bgMusic
        +load()
    }

    class InputHandler {
        +handleInput()
    }

    class GuiLayer {
        +update(count)
        +render()
    }

    %% Relationships
    Game *-- PhysicsWorld
    Game *-- InputHandler
    Game *-- GuiLayer
    Game *-- GameResources
    Game *-- "1" Sun
    Game *-- "*" Planet

    CelestialBody <|-- Sun : Inherits
    CelestialBody <|-- Planet : Inherits

    Game ..> GravitySystem : Uses
    PhysicsWorld *-- ContactListener : Owns
