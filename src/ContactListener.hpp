#pragma once
#include <box2d/box2d.h>
#include "Planet.hpp"
#include "Sun.hpp" // We need to know what a Sun is

class ContactListener : public b2ContactListener {
public:
    // Called when two shapes start touching
    void BeginContact(b2Contact* contact) override {
        handleCollision(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody());
    }

    // Called BEFORE physics calculates the bounce.
    // We use this to DISABLE the bounce physics for planets colliding.
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // Check if collision involves a Planet
        bool aIsPlanet = (bodyA->GetType() == b2_dynamicBody);
        bool bIsPlanet = (bodyB->GetType() == b2_dynamicBody);

        // If a Planet hits the Sun (Static) OR another Planet (Dynamic)
        // We disable the physical reaction so they sink into each other instead of bouncing
        if (aIsPlanet || bIsPlanet) {
            contact->SetEnabled(false);
        }
    }

private:
    void handleCollision(b2Body* bodyA, b2Body* bodyB) {
        // Logic to trigger death animation
        // 1. Check Body A
        if (bodyA->GetType() == b2_dynamicBody) {
            auto planet = reinterpret_cast<Planet*>(bodyA->GetUserData().pointer);
            if (planet) planet->startDeath();
        }

        // 2. Check Body B
        if (bodyB->GetType() == b2_dynamicBody) {
            auto planet = reinterpret_cast<Planet*>(bodyB->GetUserData().pointer);
            if (planet) planet->startDeath();
        }
    }
};