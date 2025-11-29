#include "PhysicsWorld.hpp"
#include "Config.hpp"

PhysicsWorld::PhysicsWorld() : m_world(b2Vec2(0.f, 0.f)) {
    // Register the listener
    m_world.SetContactListener(&m_contactListener);
}

b2World& PhysicsWorld::getWorld() { return m_world; }

void PhysicsWorld::update() {
    m_world.Step(TIME_STEP, VELOCITY_ITERS, POSITION_ITERS);
}