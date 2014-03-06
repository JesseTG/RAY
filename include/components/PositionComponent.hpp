#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include <anax/anax.hpp>
#include <SFML/System.hpp>
#include <LuaContext.hpp>

#include <iostream>

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
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c pos uses. It must be implicitly convertible to a @c float.
     */
    template<class NumberType>
    PositionComponent(const NumberType x, const NumberType y) : position(x, y) {}

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
    Vector2f position;

    static void luaInit(LuaContext& lua) {
        lua.registerMember<PositionComponent, Vector2f>("position",
        [](const PositionComponent& p) {
            return p.position;
        },
        [](PositionComponent& p, const Vector2f& vec) {
            p.position = vec;
        });
        lua.registerMember<PositionComponent, float>("x",
        [](const PositionComponent& p) {
            return p.position.x;
        },
        [](PositionComponent& p, const float val) {
            p.position.x = val;
        });

        lua.registerMember<PositionComponent, float>("y",
        [](const PositionComponent& p) {
            return p.position.y;
        },
        [](PositionComponent& p, const float val) {
            p.position.y = val;
        });
    }
};
}

#endif // POSITIONCOMPONENT_HPP
