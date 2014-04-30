#include "systems/FourWayControlSystem.hpp"
#include <iostream>

#include "components.hpp"
#include "util.hpp"

namespace ray {

const ComponentFilter FourWayControlSystem::FILTER = ComponentFilter()
        .requires<FourWayControlComponent, PhysicsBodyComponent>()
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
        FourWayControlComponent& fwcc = e.getComponent<FourWayControlComponent>();
        PhysicsBodyComponent& pb   = e.getComponent<PhysicsBodyComponent>();

        b2Vec2 velocity(0, 0);
        if (Keyboard::isKeyPressed(fwcc.left)) {
            // If the user is trying to go left...
            velocity.x = -fwcc.targetSpeed;
        }
        else if (Keyboard::isKeyPressed(fwcc.right)) {
            // If the user is trying to go right...
            velocity.x = fwcc.targetSpeed;
        }
        else {
            velocity.x = 0;
        }

        if (Keyboard::isKeyPressed(fwcc.up)) {
            // If the user is trying to go up...
            velocity.y = -fwcc.targetSpeed;
        }
        else if (Keyboard::isKeyPressed(fwcc.down)) {
            // If the user is trying to go down...
            velocity.y = fwcc.targetSpeed;
        }
        else {
            velocity.y = 0;
        }

        pb.body->ApplyForceToCenter(velocity, true);
    }
}

}
