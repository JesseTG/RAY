#include "PhysicsSystem.hpp"

#include "components.hpp"
#include "config.hpp"

namespace ray {

using anax::Entity;

const ComponentFilter PhysicsSystem::FILTER = ComponentFilter()
        .requiresOneOf<PhysicsBodyComponent, PhysicsFixtureComponent>();

PhysicsSystem::PhysicsSystem(b2World& world) : Base(FILTER), _world(&world)
{
    //ctor
}

PhysicsSystem::PhysicsSystem(b2World* world) : Base(FILTER), _world(world) {
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::update() {
    this->_world->Step(SPF, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
/*
    for (Entity& e : this->getEntities()) {
        PositionComponent& posc = e.getComponent<PositionComponent>();
        if (e.hasComponent<PhysicsBodyComponent>()) {
            PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();
            const b2Vec2& pos = pbc.body->GetPosition();
            posc.position.x = PIXELS_PER_METER * pos.x;
            posc.position.y = PIXELS_PER_METER * pos.y;
        }
        else if (e.hasComponent<PhysicsFixtureComponent>()) {
            PhysicsFixtureComponent& pfc = e.getComponent<PhysicsFixtureComponent>();
            const b2Vec2& pos = pfc.fixture->GetBody()->GetPosition();
            posc.position.x = PIXELS_PER_METER * pos.x;
            posc.position.y = PIXELS_PER_METER * pos.y;
        }

    }
    */
}

}
