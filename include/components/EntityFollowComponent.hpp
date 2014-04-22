#ifndef ENTITYFOLLOWCOMPONENT_HPP
#define ENTITYFOLLOWCOMPONENT_HPP

#include <anax/anax.hpp>
#include <LuaContext.hpp>
#include <cmath>

namespace ray {

using anax::Entity;

/**
 * An @c Entity with this @c Component will always follow another @c Entity. If
 * the target @c Entity no longer exists, @c *this has no effect on its @c Entity.
 *
 * The target @c Entity must have a PositionComponent!
 */
struct EntityFollowComponent : public anax::Component<EntityFollowComponent>
{
    /**
     * Constructs an EntityFollowComponent that is not following another @c Entity.
     */
    EntityFollowComponent() : maxSpeed(INFINITY) {}

    /**
     * Constructs an EntityFollowComponent that is following a given @c Entity.
     *
     * @param e The @c Entity that @c *this's @c Entity will follow.
     * @c e.isValid() must be true, and @c e must have a PositionComponent.
     */
    EntityFollowComponent(const Entity& e) : target(e), maxSpeed(INFINITY) {}

    EntityFollowComponent(const Entity& e, const float maxSpeed) : target(e), maxSpeed(maxSpeed) {}

    /**
     * The @c Entity that the @c Entity containing @c *this will follow. If
     * @c target.isValid() is @c false, @c *this has no effect on its @c Entity.
     */
    Entity target;

    /**
     * The maximum speed an Entity will follow another.  Defaults to zero
     */
    float maxSpeed;

    static void luaInit(LuaContext& lua);
};
}

#endif // ENTITYFOLLOWCOMPONENT_HPP
