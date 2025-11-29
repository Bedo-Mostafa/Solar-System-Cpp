#pragma once
#include <SFML/Graphics.hpp>
#include "GameResources.hpp"
#include <string>

class GuiLayer {
public:
    GuiLayer(const GameResources& resources) {
        m_text.setFont(resources.font);
        m_text.setCharacterSize(24);
        m_text.setFillColor(sf::Color(200, 200, 255)); // Light blue text
        m_text.setPosition(20.f, 20.f); // Top left corner
    }

    void update(int planetCount) {
        std::string info = 
            "--- SOLAR SIMULATION ---\n"
            "Physics Equation (Newton):\n"
            "F = G * (m1 * m2) / r^2\n\n"
            "Planets: " + std::to_string(planetCount) + "\n"
            "FPS: 60 (Fixed)";
        
        m_text.setString(info);
    }

    void render(sf::RenderWindow& window) {
        // Draw a semi-transparent black box behind text for readability
        sf::FloatRect bounds = m_text.getGlobalBounds();
        sf::RectangleShape bg(sf::Vector2f(bounds.width + 20.f, bounds.height + 20.f));
        bg.setFillColor(sf::Color(0, 0, 0, 150));
        bg.setPosition(0.f, 0.f);
        
        window.draw(bg);
        window.draw(m_text);
    }

private:
    sf::Text m_text;
};