#pragma once
#include "Config.hpp"
#include "CelestialBody.hpp"
#include "Planet.hpp"
#include <cmath>

class GravitySystem {
public:
    static void applyGravity(CelestialBody* sun, Planet* planet) {
        b2Vec2 diff = sun->getPosition() - planet->getPosition();
        float distSq = diff.LengthSquared();
        float dist = std::sqrt(distSq);
        diff.Normalize();

        if (planet->isDying()) {
            // Black hole suction effect
            planet->getBody()->ApplyForceToCenter(50.0f * diff, true);
            return;
        }

        if (dist < (SUN_RADIUS * METERS_PER_PIXEL)) return;

        float forceMag = G_CONSTANT * (sun->getMass() * planet->getMass()) / distSq;
        planet->getBody()->ApplyForceToCenter(forceMag * diff, true);
    }

    static b2Vec2 calculateOrbitVelocity(CelestialBody* sun, b2Vec2 planetPos) {
        b2Vec2 diff = planetPos - sun->getPosition();
        float r = diff.Length();
        float v = std::sqrt((G_CONSTANT * sun->getMass()) / r);
        b2Vec2 tangent(-diff.y, diff.x);
        tangent.Normalize();
        return v * tangent;
    }
};