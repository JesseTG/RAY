#ifndef VELOCITYCOMPONENT_HPP
#define VELOCITYCOMPONENT_HPP

#include <anax/anax.hpp>
#include <SFML/System.hpp>

namespace ray
{

using anax::Component;
using sf::Vector2;

/**
 * Enables an @c Entity to move around with some form of motion.
 */
struct VelocityComponent : public Component<VelocityComponent>
{
    /**
     * Constructs a VelocityComponent with the default velocity of @c (0, 0).
     */
    VelocityComponent() = default;

    /**
     * Constructs a VelocityComponent with given values.
     *
     * @param x The initial x (horizontal) velocity
     * @param y The initial y (vertical) velocity
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c pos uses. It must be implicitly convertible to a @c float.
     */
    template<class NumberType>
    VelocityComponent(const NumberType x, const NumberType y) : velocity(x, y) {}

    /**
     * Constructs a VelocityComponent with the given velocity vector.
     *
     * @param pos The initial velocity vector.
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c pos uses. It must be implicitly convertible to a @c float.
     */
    template<class NumberType>
    VelocityComponent(const Vector2<NumberType>& v) : velocity(v.x, v.y) {}

    /**
     * The actual @c Vector2 representing the velocity
     */
    Vector2f velocity;
};
}

#endif // VELOCITYCOMPONENT_HPP
