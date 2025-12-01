#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class PlanetType {
    MERCURY,
    VENUS,
    EARTH,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE
};

struct PlanetData {
    std::string name;
    float radius;          // Visual radius in pixels
    float density;         // For mass calculation
    sf::Color baseColor;
    sf::Color secondaryColor;  // For bands, clouds, etc.
    
    static PlanetData getData(PlanetType type) {
        switch(type) {
            case PlanetType::MERCURY:
                return {"Mercury", 20.f, 5.43f, 
                    sf::Color(169, 169, 169), 
                    sf::Color(128, 128, 128)};
            
            case PlanetType::VENUS:
                return {"Venus", 38.f, 5.24f, 
                    sf::Color(228, 180, 130), 
                    sf::Color(200, 150, 100)};
            
            case PlanetType::EARTH:
                return {"Earth", 40.f, 5.51f, 
                    sf::Color(30, 100, 200),    // Ocean blue
                    sf::Color(50, 150, 50)};    // Land green
            
            case PlanetType::MARS:
                return {"Mars", 21.f, 3.93f, 
                    sf::Color(193, 68, 14), 
                    sf::Color(150, 50, 10)};
            
            case PlanetType::JUPITER:
                return {"Jupiter", 100.f, 1.33f, 
                    sf::Color(194, 153, 107),   // Tan
                    sf::Color(160, 100, 60)};   // Brown bands
            
            case PlanetType::SATURN:
                return {"Saturn", 80.f, 0.69f, 
                    sf::Color(210, 180, 140),   // Pale gold
                    sf::Color(180, 150, 110)};
            
            case PlanetType::URANUS:
                return {"Uranus", 65.f, 1.27f, 
                    sf::Color(79, 208, 231),    // Cyan
                    sf::Color(100, 180, 200)};
            
            case PlanetType::NEPTUNE:
                return {"Neptune", 62.f, 1.64f, 
                    sf::Color(62, 84, 232),     // Deep blue
                    sf::Color(40, 60, 180)};
        }
        return getData(PlanetType::EARTH);
    }
    
    static PlanetType getNext(PlanetType current) {
        int next = (static_cast<int>(current) + 1) % 8;
        return static_cast<PlanetType>(next);
    }
};