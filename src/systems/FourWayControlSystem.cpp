#include "systems/FourWayControlSystem.hpp"
#include <iostream>

#include "components.hpp"

namespace ray {

const ComponentFilter FourWayControlSystem::FILTER = ComponentFilter()
        .requires<FourWayControlComponent, PositionComponent, VelocityComponent>()
        .excludes<MouseFollowControlComponent>();

FourWayControlSystem::FourWayControlSystem() : Base(FILTER)
{
    //ctor
}

FourWayControlSystem::~FourWayControlSystem()
{
    //dtor
}

void FourWayControlSystem::update() {
    for (Entity& e : this->getEntities()) {
        // For each Entity available...
        FourWayControlComponent& f = e.getComponent<FourWayControlComponent>();
        VelocityComponent&       v = e.getComponent<VelocityComponent>();

        if (Keyboard::isKeyPressed(f.left)) {
            // If the user is trying to go left...
            v.velocity.x = -5;
        }
        else if (Keyboard::isKeyPressed(f.right)) {
            // If the user is trying to go right...
            v.velocity.x = 5;
        }
        else {
            v.velocity.x = 0;
        }

        if (Keyboard::isKeyPressed(f.up)) {
            // If the user is trying to go up...
            v.velocity.y = -5;
        }
        else if (Keyboard::isKeyPressed(f.down)) {
            // If the user is trying to go down...
            v.velocity.y = 5;
        }
        else {
            v.velocity.y = 0;
        }
    }
}

}
