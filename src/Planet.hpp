// // Planet.hpp
// #pragma once
// #include "CelestialBody.hpp"
// #include "GameResources.hpp"
// #include <deque>
// #include <iomanip> // For std::fixed and std::setprecision
// #include <sstream>

// class Planet : public CelestialBody {
// public:
//     enum State { ALIVE, DYING, DEAD };

//     // Planet(b2World& world, sf::Vector2f position, GameResources& resources);
//     Planet(b2World& world, sf::Vector2f position, GameResources& resources) 
//         : CelestialBody(nullptr, nullptr),
//           m_shader(resources.planetShader),
//           m_trail(sf::LineStrip)
//     {
//         // ... (Keep all your existing physics/visual setup code exactly as before) ...
//         // ... (radius, color, body def, fixture def, etc.) ...

//         // --- NEW: SETUP TEXT LABEL ---
//         m_infoText.setFont(resources.font);
//         m_infoText.setCharacterSize(12); // Small text
//         m_infoText.setFillColor(sf::Color::White);
//         m_infoText.setOutlineColor(sf::Color::Black);
//         m_infoText.setOutlineThickness(1.0f);
//     }
    
//     // void update(float dt) override;
//     void update(float dt) override {
//         // ... (Keep existing Death Animation logic) ...
//         // ... (Keep existing Trail logic) ...

//         // --- NEW: UPDATE INFO TEXT ---
//         if (m_state == ALIVE && m_body) {
//             b2Vec2 vel = m_body->GetLinearVelocity();
//             float speed = vel.Length(); // Speed in m/s
//             float mass = m_body->GetMass();

//             // Format string: "M: 10.5 \n V: 5.2"
//             std::stringstream ss;
//             ss << std::fixed << std::setprecision(1);
//             ss << "M: " << mass << "\n";
//             ss << "V: " << speed;

//             m_infoText.setString(ss.str());

//             // Position text slightly above planet
//             sf::Vector2f screenPos = Utils::toPixels(m_body->GetPosition());
//             m_infoText.setPosition(screenPos.x - 15.f, screenPos.y - 40.f);
//         }
//     }
//     // void render(sf::RenderWindow& window, sf::Shader* = nullptr) override;
//     void render(sf::RenderWindow& window, sf::Shader* = nullptr) override {
//         // 1. Draw Trail
//         window.draw(m_trail);

//         // 2. Draw Planet Body (Keep existing shader logic)
//         if (m_body && m_shape) {
//             sf::Vector2f sunPos = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
//             sf::Vector2f myPos = Utils::toPixels(m_body->GetPosition());
//             sf::Vector2f lightDir = sunPos - myPos;
            
//             float len = std::sqrt(lightDir.x*lightDir.x + lightDir.y*lightDir.y);
//             if (len > 0) lightDir /= len;

//             m_shader.setUniform("u_baseColor", sf::Glsl::Vec4(m_baseColor));
//             m_shader.setUniform("u_lightDir", lightDir);
//             m_shader.setUniform("u_dissolve", m_dissolveAmount);

//             CelestialBody::render(window, &m_shader);
//         }

//         // 3. --- NEW: Draw Info Text (On Top) ---
//         // Only draw text if alive
//         if (!isDying() && !isDead()) {
//             window.draw(m_infoText);
//         }
//     }
//     void startDeath();
//     bool isDead() const;
//     bool isDying() const;

// private:
//     sf::Shader& m_shader;
//     sf::Color m_baseColor;
//     // --- NEW VARIABLE ---
//     sf::Text m_infoText;
//     State m_state = ALIVE;
//     float m_deathTimer = 0.0f;
//     float m_dissolveAmount = 0.0f;
    
//     // Trail
//     std::deque<sf::Vertex> m_trailPoints;
//     sf::VertexArray m_trail;
//     const size_t MAX_TRAIL_POINTS = 150;
// };


#pragma once
#include "CelestialBody.hpp"
#include "GameResources.hpp"
#include <deque>
#include <iomanip> // For std::fixed and std::setprecision
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
    sf::Text m_infoText; // The text label
    
    State m_state = ALIVE;
    float m_deathTimer = 0.0f;
    float m_dissolveAmount = 0.0f;
    
    // Trail
    std::deque<sf::Vertex> m_trailPoints;
    sf::VertexArray m_trail;
    float m_trailTimer = 0.0f;
    const size_t MAX_TRAIL_POINTS = 150;
};