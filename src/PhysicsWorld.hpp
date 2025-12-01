#pragma once
#include <box2d/box2d.h>
#include <memory>
#include "ContactListener.hpp"

class PhysicsWorld {
public:
    PhysicsWorld();
    b2World& getWorld();
    void update();
private:
    b2World m_world;
    ContactListener m_contactListener;
};