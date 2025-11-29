// // Game.hpp
// #pragma once
// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <memory>
// #include "PhysicsWorld.hpp"
// #include "GameResources.hpp"
// #include "InputHandler.hpp"
// #include "Sun.hpp"
// #include "Planet.hpp"
// #include "StarBackground.hpp"
// #include "GuiLayer.hpp" // <--- Add this include

// class Game {
// public:
//     Game();
//     void run();
//     void spawnPlanet(sf::Vector2f mousePos);

// private:
//     void update(float dt);
//     void render();

//     sf::RenderWindow m_window;
//     PhysicsWorld m_physics;
//     GameResources m_resources;
    
//     std::unique_ptr<InputHandler> m_input;
//     std::unique_ptr<StarBackground> m_background;
//     // --- NEW: GUI Layer ---
//     std::unique_ptr<GuiLayer> m_gui;

//     std::unique_ptr<Sun> m_sun;
//     std::vector<std::unique_ptr<Planet>> m_planets;
//     sf::Clock m_clock;
// };


#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "PhysicsWorld.hpp"
#include "GameResources.hpp"
#include "InputHandler.hpp"
#include "Sun.hpp"
#include "Planet.hpp"
#include "StarBackground.hpp"
#include "GuiLayer.hpp"

class Game {
public:
    Game();
    void run();
    void spawnPlanet(sf::Vector2f mousePos);

    // Helper to map screen coordinates to world coordinates
    // (Needed so mouse clicks work correctly while zooming)
    sf::Vector2f getMouseWorldPos(int x, int y) const;

private:
    void update(float dt);
    void render();

    sf::RenderWindow m_window;
    
    // --- NEW: Camera View ---
    sf::View m_gameView; 
    
    // --- NEW: Intro Animation Variables ---
    float m_introTimer = 0.0f;
    // const float INTRO_DURATION = 5.0f; // 5 Seconds intro
    // bool m_introFinished = false;
    // --- CHANGED: Increased duration from 5.0f to 8.0f for a slower intro ---
    const float INTRO_DURATION = 8.0f; 
    bool m_introFinished = false;

    PhysicsWorld m_physics;
    GameResources m_resources;
    
    std::unique_ptr<InputHandler> m_input;
    std::unique_ptr<StarBackground> m_background;
    std::unique_ptr<GuiLayer> m_gui;
    std::unique_ptr<Sun> m_sun;
    std::vector<std::unique_ptr<Planet>> m_planets;
    sf::Clock m_clock;
};