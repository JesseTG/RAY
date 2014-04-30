#ifndef HEALTHCOMPONENT_HPP
#define HEALTHCOMPONENT_HPP

#include <functional>

#include <anax/anax.hpp>
#include <SFML/System.hpp>
#include <LuaContext.hpp>

namespace ray
{

using std::function;
using anax::Component;
using anax::Entity;

/**
 * tbd
 */
struct HealthComponent : public Component<HealthComponent>
{
    /**
     * Constructs a HealthComponent with the default health of @c 0.
     */
    HealthComponent() : HealthComponent(0) {}

    /**
     * Constructs a VelocityComponent with given values.
     *
     * @param hp the initial health
     * @tparam NumberType The numerical type (typically a @c float or an @c int)
     * that @c health uses. It must be implicitly convertible to a @c float.
     */
    HealthComponent(const int hp) : health(hp), max_health(hp), onDeath([](Entity e){ return e; }) {}

    float fraction() const;

    /**
     * The actual int representing the health
     */
    int health;

    int max_health;

    function<Entity(Entity)> onDeath;

    static void luaInit(LuaContext& lua);
};
}

#endif // HEALTHCOMPONENT_HPP
