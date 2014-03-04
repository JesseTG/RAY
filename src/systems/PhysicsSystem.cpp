#include "PhysicsSystem.hpp"

#include "config.hpp"

namespace ray {
PhysicsSystem::PhysicsSystem() : _world(b2Vec2(0, 0)), Base()
{
    //ctor
}

PhysicsSystem::~PhysicsSystem()
{
    //dtor
}

void PhysicsSystem::update() {
    this->_world.Step(SPF, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

}
