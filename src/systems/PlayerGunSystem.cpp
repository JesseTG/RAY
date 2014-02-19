#include "PlayerGunSystem.hpp"

#include <cmath>
#include <iostream>

#include "components.hpp"

namespace ray {

const ComponentFilter PlayerGunSystem::FILTER = ComponentFilter()
        .requires<PositionComponent, MouseFollowControlComponent>();

PlayerGunSystem::PlayerGunSystem(const Entity& player, RenderWindow& window) :
    Base(FILTER),
    _player(player),
    _window(&window)
{
    //ctor
}

PlayerGunSystem::~PlayerGunSystem()
{
    //dtor
}

void PlayerGunSystem::update(const vector<Event>& events) {
    for (const Event& e : events) {
        if (e.type == Event::MouseButtonPressed) {
            if (e.mouseButton.button == Mouse::Button::Left) {
                Vector2f worldpos =
                    this->_window->mapPixelToCoords(Vector2i(e.mouseButton.x, e.mouseButton.y));
                // Where in the world will we create the bullet?

                Entity ent = this->getWorld().createEntity();
                ent.addComponent(new PositionComponent(worldpos));
                ent.addComponent(new RenderableComponent);
                ent.addComponent(new VelocityComponent(1, 1));
                ent.addComponent(new BulletComponent(5));
                ent.activate();
            }
        }
    }
}

}
