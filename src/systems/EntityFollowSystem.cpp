#include "EntityFollowSystem.hpp"

#include "components.hpp"
#include <cmath>

namespace ray {
using std::isinf;
const ComponentFilter EntityFollowSystem::FILTER = ComponentFilter()
        .requires<EntityFollowComponent, PositionComponent>();

EntityFollowSystem::EntityFollowSystem() : Base(FILTER)
{
    //ctor
}

EntityFollowSystem::~EntityFollowSystem()
{
    //dtor
}

void EntityFollowSystem::update() {
    for (Entity& e : this->getEntities()) {
        // For each entity we have...
        EntityFollowComponent& ef = e.getComponent<EntityFollowComponent>();
        if (isinf(ef.maxSpeed) && ef.target.isValid() && ef.target.hasComponent<PositionComponent>()) {
            // If the entity we're trying to face exists and has a position...

            PositionComponent& pos = e.getComponent<PositionComponent>();
            PositionComponent& follow_pos = ef.target.getComponent<PositionComponent>();

            pos.position = follow_pos.position;
        }
        else if (e.hasComponent<PhysicsBodyComponent>()) {
            PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();

            b2Vec2 targetp = ef.target.getComponent<PhysicsBodyComponent>().body->GetPosition();
            b2Vec2 ep = pbc.body->GetPosition();
            b2Vec2 direction = targetp - ep;
            direction.Normalize();
            direction *= ef.maxSpeed;

             //pbc.body->SetLinearVelocity(direction);
        }
    }
}

}
