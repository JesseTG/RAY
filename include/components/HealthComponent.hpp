#ifndef HEALTHCOMPONENT_HPP
#define HEALTHCOMPONENT_HPP

#include <anax/anax.hpp>
#include <SFML/System.hpp>
#include <LuaContext.hpp>

namespace ray
{

using anax::Component;

/**
 * Enables an @c Entity to move around with some form of motion.
 */
struct HealthComponent : public Component<HealthComponent>
{
    /**
     * Constructs a HealthComponent with the default health of @c (0).
     */
    HealthComponent() = default;

    /**
     * Constructs a VelocityComponent with given values.
     *
     * @param hp the initial health
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c health uses. It must be implicitly convertible to a @c float.
     */
    template<class NumberType>
    HealthComponent(const NumberType hp) : health(hp) {}

    /**
     * The actual @c NumberType representing the health
     */
    float health;

    static void luaInit(LuaContext& lua);
};
}

#endif // HEALTHCOMPONENT_HPP
