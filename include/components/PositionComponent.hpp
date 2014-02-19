#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include <anax/anax.hpp>
#include <SFML/System.hpp>

namespace ray
{

using anax::Component;
using sf::Vector2;
using sf::Vector2f;
using sf::Vector2i;

/**
 * Allows a @c Entity to be positioned somewhere on the Cartesian plane.
 */
struct PositionComponent : Component<PositionComponent>
{
    /**
     * Constructs a PositionComponent with the default coordinates of @c (0, 0).
     */
    PositionComponent() = default;

    /**
     * Constructs a PositionComponent with given coordinates.
     *
     * @param x The initial x (horizontal) coordinate.
     * @param y The initial y (vertical) coordinate.
     */
    PositionComponent(const float x, const float y) : position(x, y) {}

    /**
     * Constructs a PositionComponent with given position vector.
     *
     * @param pos The initial position vector. Remember, you don't need to
     * explicitly specify the template specialization.
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c pos uses. It must be implicitly convertible to a @c float.
     */
    template<class NumberType>
    PositionComponent(const Vector2<NumberType>& pos) : position(pos.x, pos.y) {}

    /**
     * The position stored by @c *this.
     */
    Vector2<float> position;
};
}

#endif // POSITIONCOMPONENT_HPP
