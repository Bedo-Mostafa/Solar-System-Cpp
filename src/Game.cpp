#include "Game.hpp"
#include "GravitySystem.hpp"
#include <algorithm>
#include <cmath>

Game::Game() 
    : m_window(sf::VideoMode(1920, 1080), "Solar System Simulator", sf::Style::Fullscreen, sf::ContextSettings(0, 0, 8))
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

    // Initialize View
    m_gameView.setCenter(center);
    m_gameView.setSize(m_window.getDefaultView().getSize() * 0.1f);
}

void Game::run() {
    while (m_window.isOpen()) {
        m_input->handleInput(m_window);
        update(m_clock.restart().asSeconds());
        render();
    }
}

sf::Vector2f Game::getMouseWorldPos(int x, int y) const {
    return m_window.mapPixelToCoords(sf::Vector2i(x, y), m_gameView);
}

void Game::spawnPlanet(sf::Vector2f mousePos) {
    // Create planet with current type
    auto planet = std::make_unique<Planet>(m_physics.getWorld(), mousePos, m_resources, m_currentPlanetType);
    
    // Calculate orbital velocity
    b2Vec2 v = GravitySystem::calculateOrbitVelocity(m_sun.get(), planet->getPosition());
    planet->getBody()->SetLinearVelocity(v);
    
    m_planets.push_back(std::move(planet));
    
    // Cycle to next planet type
    m_currentPlanetType = PlanetData::getNext(m_currentPlanetType);
}


void Game::update(float dt) {
    m_background->update(dt);

    // Smooth intro animation
    if (!m_introFinished) {
        m_introTimer += dt;
        
        float t = std::min(m_introTimer / INTRO_DURATION, 1.0f);
        float ease = t * t * (3.0f - 2.0f * t);

        float currentZoom = 0.1f + (1.0f - 0.1f) * ease;
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
    
    // Draw Background
    m_window.setView(m_window.getDefaultView());
    m_background->render(m_window);

    // Draw Game Objects
    m_window.setView(m_gameView);
    m_sun->render(m_window);
    for (auto& p : m_planets) p->render(m_window);
    
    // Draw GUI
    m_window.setView(m_window.getDefaultView());
    m_gui->render(m_window);

    m_window.display();
}