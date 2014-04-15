#include "systems/RenderSystem.hpp"

#include <algorithm>
#include <iostream>

namespace ray {

using std::sort;

const ComponentFilter RenderSystem::FILTER = ComponentFilter()
        .requires<RenderableComponent, PositionComponent>();

RenderSystem::RenderSystem(RenderWindow& window) : Base(FILTER), _window(&window)
{
}

void RenderSystem::update() {
    auto entities = this->getEntities();
    sort(entities.begin(), entities.end(), &RenderSystem::_sort_entities);
    this->_window->clear();

    for (Entity& e : entities) {
        RenderableComponent& graphic = e.getComponent<RenderableComponent>();
        PositionComponent&   p       = e.getComponent<PositionComponent>();

        if (graphic.transformable) {
            // If this render item can be transformed...
            // TODO: Figure out if SFML handles clipping under the hood
            graphic.transformable->setPosition(p.position);
        }
        this->_window->draw(*(graphic.drawable), graphic.render_states);
    }
    this->_window->display();
}

bool RenderSystem::_sort_entities(const Entity& e1, const Entity& e2) noexcept {
    RenderableComponent& r1 = e1.getComponent<RenderableComponent>();
    RenderableComponent& r2 = e2.getComponent<RenderableComponent>();

    return r1.layer < r2.layer;
}

}

