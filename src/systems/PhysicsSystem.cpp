#include "PhysicsSystem.hpp"

#include "components.hpp"
#include "config.hpp"

#include <iostream>
#include <cassert>

namespace ray {

using anax::Entity;

const ComponentFilter PhysicsSystem::FILTER = ComponentFilter()
        .requiresOneOf<PhysicsBodyComponent, PhysicsFixtureComponent>();

PhysicsSystem::PhysicsSystem(shared_ptr<b2World> world) : Base(FILTER), _world(world)
{
    //ctor
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::onEntityAdded(Entity& entity) {
}

void PhysicsSystem::onEntityRemoved(Entity& entity) {
    /*
    if (entity.hasComponent<PhysicsBodyComponent>()) {
        PhysicsBodyComponent& pbc = entity.getComponent<PhysicsBodyComponent>();
        assert(this->_world.lock().get() == pbc.body->GetWorld());
        pbc.body->SetActive(false);
        pbc.body = nullptr;
    }
    */
}

void PhysicsSystem::update() {
    auto ptr = this->_world.lock();
    ptr->Step(SPF, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

}
