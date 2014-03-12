#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include <iostream>
#include <exception>
#include <string>

#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>
#include <SFML/System.hpp>
#include <LuaContext.hpp>


namespace ray
{
using std::invalid_argument;
using std::string;
using boost::get;
using boost::optional;
using boost::variant;
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

    PositionComponent(const b2Vec2& pos) : position(pos.x, pos.y) {}

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

    static void luaInit(LuaContext& lua);
};
}

#endif // POSITIONCOMPONENT_HPP

