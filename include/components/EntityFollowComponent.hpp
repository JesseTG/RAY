#ifndef ENTITYFOLLOWCOMPONENT_HPP
#define ENTITYFOLLOWCOMPONENT_HPP

#include <anax/anax.hpp>
#include <LuaContext.hpp>

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
    EntityFollowComponent() {}

    /**
     * Constructs an EntityFollowComponent that is following a given @c Entity.
     *
     * @param e The @c Entity that @c *this's @c Entity will follow.
     * @c e.isValid() must be true, and @c e must have a PositionComponent.
     */
    EntityFollowComponent(const Entity& e) : target(e) {}

    /**
     * The @c Entity that the @c Entity containing @c *this will follow. If
     * @c target.isValid() is @c false, @c *this has no effect on its @c Entity.
     */
    Entity target;

    static void luaInit(LuaContext& lua) {
        lua.registerMember("target", &EntityFollowComponent::target);
    }
};
}

#endif // ENTITYFOLLOWCOMPONENT_HPP
