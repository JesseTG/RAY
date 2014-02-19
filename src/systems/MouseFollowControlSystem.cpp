#include "MouseFollowControlSystem.hpp"

#include <cmath>

#include "config.hpp"
#include "util.hpp"

namespace ray {

const ComponentFilter MouseFollowControlSystem::FILTER = ComponentFilter()
        .requires<MouseFollowControlComponent, PositionComponent>()
        .excludes<FourWayControlComponent>();

MouseFollowControlSystem::MouseFollowControlSystem(RenderWindow& window) :
    Base(FILTER),
    _window(&window)
{
    //ctor
}

MouseFollowControlSystem::~MouseFollowControlSystem()
{
    //dtor
}

void MouseFollowControlSystem::update() {
    for (Entity& e : this->getEntities()) {
        // For each Entity we have...
        PositionComponent& p = e.getComponent<PositionComponent>();
        Vector2i           m = Mouse::getPosition(*(this->_window));
        // Window coordinates

        Vector2f mapped = this->_window->mapPixelToCoords(m);
        // World coordinates

        p.position.x = constrain(mapped.x, mapped.x - m.x, mapped.x + SCREEN_SIZE.x - m.x);
        p.position.y = constrain(mapped.y, mapped.y - m.y, mapped.y + SCREEN_SIZE.y - m.y);
    }
}

}
