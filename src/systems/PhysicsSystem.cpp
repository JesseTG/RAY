#include "PhysicsSystem.hpp"

#include "components.hpp"
#include "config.hpp"

namespace ray {

using anax::Entity;

const ComponentFilter PhysicsSystem::FILTER = ComponentFilter()
        .requires<PhysicsBodyComponent, PositionComponent>();

PhysicsSystem::PhysicsSystem(b2World& world) : Base(FILTER), _world(&world)
{
    //ctor
}

PhysicsSystem::~PhysicsSystem()
{
    //dtor
}

void PhysicsSystem::update() {
    this->_world->Step(SPF, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

    for (Entity& e : this->getEntities()) {
        PhysicsBodyComponent& phy = e.getComponent<PhysicsBodyComponent>();
        PositionComponent& posc = e.getComponent<PositionComponent>();
        const b2Vec2& pos = phy.body->GetPosition();

        posc.position.x = pos.x;
        posc.position.y = pos.y;
    }
}

}
