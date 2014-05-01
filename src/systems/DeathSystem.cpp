#include "DeathSystem.hpp"

namespace ray {

const ComponentFilter DeathSystem::FILTER = ComponentFilter()
        .requires<HealthComponent>();

DeathSystem::DeathSystem() : Base(FILTER)
{
    //ctor
}

DeathSystem::~DeathSystem()
{
    //dtor
}

void DeathSystem::update() {
    for (Entity& e : this->getEntities()) {
        HealthComponent& hc = e.getComponent<HealthComponent>();
        if (hc._alive && hc.health <= 0) {
            // If this entity can be considered dead...
            hc._alive = false;
            hc.onDeath(e);
        }
    }
}

}
