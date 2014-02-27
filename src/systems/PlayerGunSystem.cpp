#include "PlayerGunSystem.hpp"

#include <cmath>
#include <iostream>

#include "entities.hpp"
#include "components.hpp"

namespace ray {

using std::hypot;
using std::cos;
using std::sin;
using std::atan2;

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
                // Where in the world will the bullet fly to?

                Vector2f playerpos = this->_player.getComponent<PositionComponent>().position;
                Vector2f v = worldpos - playerpos;
                double theta = atan2(v.y, v.x);
                double vx = 5 * cos(theta);
                double vy = 5 * sin(theta);

                entities::createBullet(playerpos.x, playerpos.y, vx, vy);
                entities::createEnemy(worldpos.x, worldpos.y, 8);
            }
        }
    }
}

}
