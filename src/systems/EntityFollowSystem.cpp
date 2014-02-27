#include "EntityFollowSystem.hpp"

#include "components.hpp"

namespace ray {

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
        if (ef.target.isValid() && ef.target.hasComponent<PositionComponent>()) {
            // If the entity we're trying to face exists and has a position...

            PositionComponent& pos = e.getComponent<PositionComponent>();
            PositionComponent& follow_pos = ef.target.getComponent<PositionComponent>();

            pos.position = follow_pos.position;
        }
    }
}

}
