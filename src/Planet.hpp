#pragma once
#include "CelestialBody.hpp"
#include "GameResources.hpp"
#include "PlanetType.hpp"
#include <deque>
#include <iomanip>
#include <sstream>

class Planet : public CelestialBody {
public:
    enum State { ALIVE, DYING, DEAD };

    // Constructor with planet type
    Planet(b2World& world, sf::Vector2f position, GameResources& resources, PlanetType type);
    
    // Override Methods
    void update(float dt) override;
    void render(sf::RenderWindow& window, sf::Shader* = nullptr) override;
    
    // Class Methods
    void startDeath();
    bool isDead() const;
    bool isDying() const;
    PlanetType getType() const { return m_type; }

private:
    PlanetType m_type;
    PlanetData m_data;
    sf::Shader* m_shader;
    sf::Color m_baseColor;
    sf::Color m_secondaryColor;
    sf::Text m_infoText;
    
    State m_state = ALIVE;
    float m_deathTimer = 0.0f;
    float m_dissolveAmount = 0.0f;
    float m_animationTime = 0.0f;  // For animated shaders
    
    // Trail
    std::deque<sf::Vertex> m_trailPoints;
    sf::VertexArray m_trail;
    float m_trailTimer = 0.0f;
    const size_t MAX_TRAIL_POINTS = 150;
};