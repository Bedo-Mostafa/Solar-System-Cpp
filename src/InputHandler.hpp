// InputHandler.hpp
#pragma once
#include <SFML/Graphics.hpp>
class Game;

class InputHandler {
public:
    InputHandler(Game& game);
    void handleInput(sf::RenderWindow& window);
private:
    Game& m_game;
};