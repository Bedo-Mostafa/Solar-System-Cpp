#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <memory>
#include "Utils.hpp"

class CelestialBody {
public:
    CelestialBody(b2Body* body, std::shared_ptr<sf::Shape> shape) 
        : m_body(body), m_shape(shape) {}

    virtual ~CelestialBody() = default;

    b2Body* getBody() { return m_body; }
    virtual float getMass() const { return m_body->GetMass(); }
    virtual b2Vec2 getPosition() const { return m_body->GetPosition(); }
    virtual void update(float dt) {}

    virtual void render(sf::RenderWindow& window, sf::Shader* shader = nullptr) {
        if (m_body && m_shape) {
            m_shape->setPosition(Utils::toPixels(m_body->GetPosition()));
            if (shader) window.draw(*m_shape, shader);
            else window.draw(*m_shape);
        }
    }

protected:
    b2Body* m_body;
    std::shared_ptr<sf::Shape> m_shape;
};