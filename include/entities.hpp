#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <SFML/Graphics.hpp>
#include <anax/anax.hpp>

namespace ray {
namespace entities {
using anax::Entity;
using anax::World;

using sf::CircleShape;
using sf::Color;
using sf::RenderWindow;

extern World*        _world ;
extern RenderWindow* _window;

/**
 * Sets the @c World that will create the @c Entities (so you don't have to
 * keep passing in the @c World as a parameter to each factory function).
 *
 * @param World The @c World that created @c Entities will belong to
 */
void setWorld(World&) noexcept;

void setRenderWindow(RenderWindow&) noexcept;

Entity createKeyboardCircle(float, float, float) noexcept;

Entity createMouseCircle(float) noexcept;

}
}


#endif // ENTITIES_HPP
