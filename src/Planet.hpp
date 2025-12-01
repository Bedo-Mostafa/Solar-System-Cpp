#pragma once
#include "CelestialBody.hpp"
#include "GameResources.hpp"
#include <deque>
#include <iomanip>
#include <sstream>

class Planet : public CelestialBody {
public:
    enum State { ALIVE, DYING, DEAD };

    // Constructor Declaration
    Planet(b2World& world, sf::Vector2f position, GameResources& resources);
    
    // Override Methods Declaration
    void update(float dt) override;
    void render(sf::RenderWindow& window, sf::Shader* = nullptr) override;
    
    // Class Methods
    void startDeath();
    bool isDead() const;
    bool isDying() const;

private:
    sf::Shader& m_shader;
    sf::Color m_baseColor;
    sf::Text m_infoText;
    
    State m_state = ALIVE;
    float m_deathTimer = 0.0f;
    float m_dissolveAmount = 0.0f;
    
    // Trail
    std::deque<sf::Vertex> m_trailPoints;
    sf::VertexArray m_trail;
    float m_trailTimer = 0.0f;
    const size_t MAX_TRAIL_POINTS = 150;
};