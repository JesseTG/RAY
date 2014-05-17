#include "DeathSystem.hpp"

namespace ray {

const ComponentFilter DeathSystem::FILTER = ComponentFilter()
        .requires<HealthComponent>();

DeathSystem::DeathSystem(){}

DeathSystem::DeathSystem(GameManager *gm) : Base(FILTER), gm(gm)
{
    //ctor
}

DeathSystem::~DeathSystem()
{
    //dtor
}

void DeathSystem::update() {
    for (Entity& e : this->getEntities()) {
        if (e.hasComponent<HealthComponent>()) {
            HealthComponent& hc = e.getComponent<HealthComponent>();
            if (hc._alive && hc.health <= 0) {
                // If this entity can be considered dead...
                if (e.hasComponent<TypeComponent>() && e.getComponent<TypeComponent>().type == 0) { // enemy
                    gm->enemiesKilled = gm->enemiesKilled + 1;
                }
                hc._alive = false;
                hc.onDeath(e);
            }
        }
    }
}

}
