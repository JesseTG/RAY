#include "PhysicsSystem.hpp"

#include "components.hpp"
#include "config.hpp"

namespace ray {

using anax::Entity;

const ComponentFilter PhysicsSystem::FILTER = ComponentFilter()
        .requires<PhysicsBodyComponent, PositionComponent, VelocityComponent>();

PhysicsSystem::PhysicsSystem(b2World& world) : _world(&world), Base(FILTER)
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
        VelocityComponent& velc = e.getComponent<VelocityComponent>();
        const b2Vec2& pos = phy.body->GetPosition();
        const b2Vec2& vel = phy.body->GetLinearVelocity();

        posc.position.x = pos.x;
        posc.position.y = pos.y;
        velc.velocity.x = vel.x;
        velc.velocity.y = vel.y;
    }
}

}
