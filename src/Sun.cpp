
// Sun.cpp
#include "Sun.hpp"
#include "Config.hpp"

Sun::Sun(b2World& world, sf::Vector2f position, GameResources& resources)
    : CelestialBody(nullptr, nullptr), m_shader(resources.sunShader)
{
    // Initialize Logic
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position = Utils::toMeters(position);
    m_body = world.CreateBody(&def);
    
    b2CircleShape b2shape;
    b2shape.m_radius = SUN_RADIUS * METERS_PER_PIXEL;
    m_body->CreateFixture(&b2shape, 0.0f);

    // Initialize Visuals
    float visualRadius = SUN_RADIUS * 1.5f; 
    auto shape = std::make_shared<sf::CircleShape>(visualRadius);
    shape->setOrigin(visualRadius, visualRadius);
    shape->setFillColor(sf::Color::White);
    shape->setTexture(&resources.dummyTexture);
    m_shape = shape;

    m_mass = SUN_MASS;
    m_shader.setUniform("u_color", sf::Glsl::Vec4(1.0, 0.6, 0.1, 1.0)); 
}

float Sun::getMass() const { return m_mass; }

void Sun::render(sf::RenderWindow& window, sf::Shader*) {
    CelestialBody::render(window, &m_shader);
}