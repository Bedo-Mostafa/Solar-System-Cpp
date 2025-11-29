# 🌌 Polished Solar System Simulation

A high-performance, physics-based space simulation built with **C++**, **SFML**, and **Box2D**. 

This project simulates Newtonian gravity, orbital mechanics, and celestial collisions, rendered with custom GLSL shaders for a cinematic visual experience. The architecture follows **SOLID principles** for modularity and scalability.

![Project Screenshot](screenshot.png) 
*(Add a screenshot of your game here)*

## ✨ Key Features

* **Real-time Physics:** Uses **Box2D** for collision detection and custom Newtonian gravity calculations.
* **Procedural Generation:** Every planet has randomized mass, radius, density, and color.
* **Cinematic Visuals:**
    * **Milky Way Shader:** A dynamic, procedurally generated galaxy background.
    * **3D Planet Lighting:** Custom fragment shaders make 2D circles look like 3D spheres with rim lighting.
    * **Glowing Sun:** Radial gradient shader for the sun's core and corona.
    * **Trails:** Fading vertex arrays to visualize orbital paths.
* **Interactive Simulation:**
    * **Spawn System:** Click to launch planets into stable orbit.
    * **Destruction Physics:** Planets are pulled into the sun or collide with each other (no bouncing, realistic absorption).
    * **Cinematic Intro:** Smooth camera zoom-out animation on startup.
* **Live Dashboard:** Real-time display of physics equations, entity count, and individual planet stats (Mass/Velocity).

## 🛠️ Tech Stack & Architecture

* **Language:** C++17
* **Graphics:** SFML 2.6.1
* **Physics:** Box2D 2.4.1
* **Build System:** CMake (Fully Automated)

### Architecture (SOLID Principles)
The codebase is structured to ensure Single Responsibility and Dependency Inversion:

* **`GameResources`:** Singleton-like resource manager for Shaders and Fonts (Flyweight pattern).
* **`PhysicsWorld`:** Encapsulates the Box2D world simulation step.
* **`CelestialBody`:** Abstract base class for all renderable physics objects (Liskov Substitution).
* **`GravitySystem`:** Pure static class handling Newton's Law of Universal Gravitation ($F = G \frac{m_1 m_2}{r^2}$).
* **`InputHandler`:** Separates event polling from game logic.
* **`ContactListener`:** Observer pattern implementation for handling collisions (Planet death/merging).

## 🚀 How to Build

This project uses a **Self-Healing CMake Script**. It automatically downloads dependencies (if configured) or patches local libraries and copies all necessary DLLs to the build folder.

### Prerequisites
* **C++ Compiler:** GCC 15+ (MinGW/MSYS2 Recommended) or MSVC.
* **CMake:** Version 3.16 or higher.
* **MSYS2 Packages (For Audio support):** `libflac`, `libvorbis`, `libogg`, `libopenal`.

### Build Steps (VS Code)
1.  Clone the repository:
    ```bash
    git clone [https://github.com/YourUsername/Solar-System-Cpp.git](https://github.com/YourUsername/Solar-System-Cpp.git)
    ```
2.  Open the folder in **VS Code**.
3.  Ensure the **CMake Tools** extension is installed.
4.  Select your Kit (e.g., `GCC 15.2.0 x86_64-w64-mingw32`).
5.  Press **F7** to Build.
6.  Press **F5** to Run.

*Note: The CMake script will automatically find your compiler's runtime DLLs and the SFML/Box2D DLLs and copy them next to the executable.*

## 🎮 Controls

* **Mouse Left Click:** Spawn a new planet at the cursor location.
* The planet will automatically calculate the velocity required for a stable circular orbit around the Sun.

## 📂 Project Structure

```text
src/
├── Config.hpp           # Physics constants and simulation tuning
├── Game.cpp             # Main game loop and rendering orchestrator
├── GravitySystem.hpp    # Newtonian physics logic
├── CelestialBody.hpp    # Base class for Sun and Planets
├── Planet.cpp           # Planet logic (Trails, Death animation, UI)
├── Sun.cpp              # Sun logic (Shader uniforms)
├── StarBackground.hpp   # Procedural GLSL background generator
├── GameResources.hpp    # Resource loader (Shaders/Fonts/Textures)
├── PhysicsWorld.cpp     # Box2D wrapper
└── ...