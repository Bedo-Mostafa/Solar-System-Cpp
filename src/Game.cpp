#include "Game.hpp"
#include "GravitySystem.hpp"
#include <algorithm>
#include <cmath>

Game::Game() 
    : m_window(sf::VideoMode(1400, 1000), "Polished Solar System", sf::Style::Default, sf::ContextSettings(0, 0, 8))
{
    m_window.setFramerateLimit(60);
    m_resources.load();
    
    if (m_resources.bgMusic.getDuration().asMilliseconds() > 0) {
        m_resources.bgMusic.play();
    }

    m_input = std::make_unique<InputHandler>(*this);
    m_background = std::make_unique<StarBackground>((float)m_window.getSize().x, (float)m_window.getSize().y);
    m_gui = std::make_unique<GuiLayer>(m_resources);

    sf::Vector2f center(m_window.getSize().x/2.f, m_window.getSize().y/2.f);
    m_sun = std::make_unique<Sun>(m_physics.getWorld(), center, m_resources);

    // --- NEW: Initialize View ---
    // Start centered on the screen
    m_gameView.setCenter(center);
    
    // Start VERY zoomed in (0.1 means 10% of normal size)
    // Effectively a 10x Zoom on the Sun
    m_gameView.setSize(m_window.getDefaultView().getSize() * 0.1f);
}

void Game::run() {
    while (m_window.isOpen()) {
        m_input->handleInput(m_window);
        update(m_clock.restart().asSeconds());
        render();
    }
}

// --- NEW: Convert Mouse to World Coordinates ---
// Since the camera zooms, screen pixel (0,0) is not world (0,0) anymore.
sf::Vector2f Game::getMouseWorldPos(int x, int y) const {
    return m_window.mapPixelToCoords(sf::Vector2i(x, y), m_gameView);
}

void Game::spawnPlanet(sf::Vector2f mousePos) {
    auto planet = std::make_unique<Planet>(m_physics.getWorld(), mousePos, m_resources);
    b2Vec2 v = GravitySystem::calculateOrbitVelocity(m_sun.get(), planet->getPosition());
    planet->getBody()->SetLinearVelocity(v);
    m_planets.push_back(std::move(planet));
}

// void Game::update(float dt) {
//     m_background->update(dt);

//     // --- NEW: INTRO ANIMATION LOGIC ---
//     if (!m_introFinished) {
//         m_introTimer += dt;
        
//         // Calculate progress (0.0 to 1.0)
//         float t = std::min(m_introTimer / INTRO_DURATION, 1.0f);

//         // Ease-Out Cubic Function (Starts fast, slows down at the end)
//         // Formula: 1 - (1-t)^3
//         float ease = 1.0f - std::pow(1.0f - t, 3.0f);

//         // Interpolate Zoom: Start at 0.1, End at 1.0
//         float currentZoom = 0.1f + (1.0f - 0.1f) * ease;

//         // Apply new size
//         sf::Vector2f defaultSize = m_window.getDefaultView().getSize();
//         m_gameView.setSize(defaultSize * currentZoom);

//         if (t >= 1.0f) m_introFinished = true;
//     }

//     // Physics Logic
//     for (auto& planet : m_planets) {
//         GravitySystem::applyGravity(m_sun.get(), planet.get());
//         planet->update(dt);
//     }
//     m_physics.update();

//     auto it = std::remove_if(m_planets.begin(), m_planets.end(), [&](const auto& p) {
//         if (p->isDead()) { 
//             m_physics.getWorld().DestroyBody(p->getBody()); 
//             return true; 
//         }
//         // No manual distance check needed (handled by ContactListener)
//         return false;
//     });

//     if (it != m_planets.end()) m_planets.erase(it, m_planets.end());
    
//     m_gui->update(m_planets.size());
// }

void Game::update(float dt) {
    m_background->update(dt);

    // --- NEW: SMOOTH INTRO ANIMATION ---
    if (!m_introFinished) {
        m_introTimer += dt;
        
        // Calculate progress (0.0 to 1.0)
        float t = std::min(m_introTimer / INTRO_DURATION, 1.0f);

        // --- CHANGED: Use "SmoothStep" interpolation ---
        // Formula: t * t * (3 - 2 * t)
        // This starts slow, speeds up in the middle, and slows down at the end.
        float ease = t * t * (3.0f - 2.0f * t);

        // Interpolate Zoom: Start at 0.1 (Close), End at 1.0 (Far)
        float currentZoom = 0.1f + (1.0f - 0.1f) * ease;

        // Apply new size
        sf::Vector2f defaultSize = m_window.getDefaultView().getSize();
        m_gameView.setSize(defaultSize * currentZoom);

        if (t >= 1.0f) m_introFinished = true;
    }

    // Physics Logic
    for (auto& planet : m_planets) {
        GravitySystem::applyGravity(m_sun.get(), planet.get());
        planet->update(dt);
    }
    m_physics.update();

    // Cleanup Logic
    auto it = std::remove_if(m_planets.begin(), m_planets.end(), [&](const auto& p) {
        if (p->isDead()) { 
            m_physics.getWorld().DestroyBody(p->getBody()); 
            return true; 
        }
        return false;
    });

    if (it != m_planets.end()) m_planets.erase(it, m_planets.end());
    
    m_gui->update(m_planets.size());
}
void Game::render() {
    m_window.clear(sf::Color::Black);
    
    // 1. Draw Background (Static - Uses Default View)
    m_window.setView(m_window.getDefaultView());
    m_background->render(m_window);

    // 2. Draw Game Objects (Zooming - Uses Game View)
    m_window.setView(m_gameView); // <--- Apply the zoom
    m_sun->render(m_window);
    for (auto& p : m_planets) p->render(m_window);
    
    // 3. Draw GUI (Static - Uses Default View)
    m_window.setView(m_window.getDefaultView()); // <--- Reset to Draw UI on top
    m_gui->render(m_window);

    m_window.display();
}