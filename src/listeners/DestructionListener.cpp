#include "DestructionListener.hpp"
#include <anax/anax.hpp>
#include "components.hpp"

namespace ray {
using anax::Entity;
DestructionListener::DestructionListener()
{
    //ctor
}

DestructionListener::~DestructionListener()
{
    //dtor
}

void DestructionListener::SayGoodbye(b2Joint* joint) {
}

void DestructionListener::SayGoodbye(b2Fixture* fixture) {
    /*
    Entity* entity = static_cast<Entity*>(fixture->GetUserData());
    if (entity->isValid()) {
        if (entity->hasComponent<PhysicsFixtureComponent>()) {
            PhysicsFixtureComponent& pfc = entity->getComponent<PhysicsFixtureComponent>();
            fixture->SetUserData(nullptr);
            pfc.fixture = nullptr;
            entity->removeComponent<PhysicsFixtureComponent>();
        }
    }
    else {
        fixture->SetUserData(nullptr);
    }
    */
}

}
