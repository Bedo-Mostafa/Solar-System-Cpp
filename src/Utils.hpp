#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "Config.hpp"
#include <random>

class Utils {
public:
    static sf::Vector2f toPixels(const b2Vec2& v) {
        return { v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER };
    }

    static b2Vec2 toMeters(const sf::Vector2f& v) {
        return { v.x * METERS_PER_PIXEL, v.y * METERS_PER_PIXEL };
    }

    static float randomFloat(float min, float max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};