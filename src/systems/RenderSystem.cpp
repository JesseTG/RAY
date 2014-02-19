#include "systems/RenderSystem.hpp"

namespace ray
{

const ComponentFilter RenderSystem::FILTER = ComponentFilter().requires<RenderableComponent, PositionComponent>();

RenderSystem::RenderSystem(RenderWindow& window) : Base(FILTER), _window(&window)
{
}

void RenderSystem::update() {
    this->_window->clear();
    for (Entity& e : this->getEntities()) {
        RenderableComponent& graphic = e.getComponent<RenderableComponent>();
        PositionComponent&   p       = e.getComponent<PositionComponent>();

        if (graphic.transformable) {
            graphic.transformable->setPosition(p.position);
        }
        this->_window->draw(*(graphic.drawable));
    }
    this->_window->display();
}
}
