#include "InputHandler.hpp"
#include "Game.hpp"

InputHandler::InputHandler(Game &game) : m_game(game) {}

void InputHandler::handleInput(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            // --- Use the Game's coordinate converter ---
            sf::Vector2f worldPos = m_game.getMouseWorldPos(event.mouseButton.x, event.mouseButton.y);
            m_game.spawnPlanet(worldPos);
        }
    }
}