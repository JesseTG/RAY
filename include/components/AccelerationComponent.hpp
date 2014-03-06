#ifndef ACCELERATIONCOMPONENT_HPP
#define ACCELERATIONCOMPONENT_HPP

#include <anax/anax.hpp>
#include <SFML/System.hpp>
#include <LuaContext.hpp>

namespace ray
{

using anax::Component;
using sf::Vector2;
using sf::Vector2i;
using sf::Vector2f;
using sf::Vector2u;

/**
 * Enables an @c Entity to have acceleration
 */
struct AccelerationComponent : public Component<AccelerationComponent>
{
    /**
     * Constructs an AccelerationComponent with the default acceleration of @c (0, 0).
     */
    AccelerationComponent() = default;

    /**
     * Constructs an AccelerationComponent with given values.
     *
     * @param x The initial x (horizontal) acceleration
     * @param y The initial y (vertical) acceleration
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c pos uses. It must be implicitly convertible to a @c float.
     */
    template<class NumberType>
    AccelerationComponent(const NumberType x, const NumberType y) : acceleration(x, y) {}

    /**
     * Constructs an AccelerationComponent with the given acceleration vector.
     *
     * @param pos The initial acceleration vector.
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c pos uses. It must be implicitly convertible to a @c float.
     */
    template<class NumberType>
    AccelerationComponent(const Vector2<NumberType>& v) : acceleration(v.x, v.y) {}

    /**
     * The actual @c Vector2 representing the acceleration
     */
    Vector2f acceleration;

    static void luaInit(LuaContext& lua) {
        lua.registerMember("acceleration", &AccelerationComponent::acceleration);
        lua.registerMember<AccelerationComponent, float>("x",
        [](const AccelerationComponent& p) -> float {
            return p.acceleration.x;
        },
        [](AccelerationComponent& p, float val) {
            p.acceleration.x = val;
        });

        lua.registerMember<AccelerationComponent, float>("y",
        [](const AccelerationComponent& p) -> float {
            return p.acceleration.y;
        },
        [](AccelerationComponent& p, float val) {
            p.acceleration.y = val;
        });
    }
};
}

#endif // ACCELERATIONCOMPONENT_HPP
