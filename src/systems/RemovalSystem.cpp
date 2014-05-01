#include "RemovalSystem.hpp"
#include "components.hpp"

#include <iostream>

namespace ray {
const ComponentFilter RemovalSystem::FILTER = ComponentFilter()
        .requires<RemovalComponent>();

RemovalSystem::RemovalSystem()// : Base(FILTER)
{
    //ctor
}

RemovalSystem::~RemovalSystem()
{
    //dtor
}

void RemovalSystem::onEntityAdded(Entity& entity) {
}

void RemovalSystem::onEntityRemoved(Entity& entity) {

}

void RemovalSystem::update() {
    for (Entity& e : this->getEntities()) {
        if (e.hasComponent<RemovalComponent>()) {
            e.deactivate();
            e.removeAllComponents();
        }
    }
}

}
