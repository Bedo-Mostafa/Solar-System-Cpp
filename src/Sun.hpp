// Sun.hpp
#pragma once
#include "CelestialBody.hpp"
#include "GameResources.hpp"

class Sun : public CelestialBody {
public:
    Sun(b2World& world, sf::Vector2f position, GameResources& resources);
    float getMass() const override;
    void render(sf::RenderWindow& window, sf::Shader* = nullptr) override;

private:
    float m_mass;
    sf::Shader& m_shader;
};
