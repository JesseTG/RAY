#include "MovementSystem.hpp"

#include "components.hpp"

namespace ray {

const ComponentFilter MovementSystem::FILTER = ComponentFilter().requires<PositionComponent, VelocityComponent>();

MovementSystem::MovementSystem() : Base(FILTER)
{
    // Note: Base is just a typedef specific to anax::System.  Normally, to
    // call a parent constructor, you have to do NameOfAParentClass(params).
    // Multiple inheritance means no super keyword.
}

MovementSystem::~MovementSystem()
{
    //dtor
}

void MovementSystem::update() {
    for (Entity& e : this->getEntities()) {
        // For each Entity we have...
        PositionComponent& p = e.getComponent<PositionComponent>();
        VelocityComponent& v = e.getComponent<VelocityComponent>();

        p.position += v.velocity;
    }
}

}
