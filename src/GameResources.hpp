#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <map>
#include "PlanetType.hpp"
#include "PlanetShaders.hpp"

struct GameResources {
    sf::Shader sunShader;
    std::map<PlanetType, sf::Shader> planetShaders;
    sf::Texture dummyTexture;
    sf::Font font;
    sf::Music bgMusic;
    bool loaded = false;

    void load() {
        if (loaded) return;

        // Create dummy texture
        sf::Image img; img.create(1, 1, sf::Color::White);
        dummyTexture.loadFromImage(img);
        dummyTexture.setSmooth(true);

        // Load Sun Shader
        sunShader.loadFromMemory(SUN_FRAG_SHADER, sf::Shader::Fragment);
        sunShader.setUniform("texture", sf::Shader::CurrentTexture);

        // Load all planet shaders
        loadPlanetShader(PlanetType::MERCURY, MERCURY_SHADER);
        loadPlanetShader(PlanetType::VENUS, VENUS_SHADER);
        loadPlanetShader(PlanetType::EARTH, EARTH_SHADER);
        loadPlanetShader(PlanetType::MARS, MARS_SHADER);
        loadPlanetShader(PlanetType::JUPITER, JUPITER_SHADER);
        loadPlanetShader(PlanetType::SATURN, SATURN_SHADER);
        loadPlanetShader(PlanetType::URANUS, URANUS_SHADER);
        loadPlanetShader(PlanetType::NEPTUNE, NEPTUNE_SHADER);

        // Load Font
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Failed to load Arial font. Make sure it exists in C:/Windows/Fonts/\n";
        }
        
        // Load Music
        if (!bgMusic.openFromFile("music.wav")) { 
            if (!bgMusic.openFromFile("src/music.wav")) {
                std::cerr << "Failed to load music.wav\n";
            }
        } else {
            bgMusic.setLoop(true);
            bgMusic.setVolume(50.0f);
        }
        
        loaded = true;
    }

private:
    void loadPlanetShader(PlanetType type, const std::string& shaderCode) {
        planetShaders[type].loadFromMemory(shaderCode, sf::Shader::Fragment);
        planetShaders[type].setUniform("texture", sf::Shader::CurrentTexture);
    }
};
