#ifndef FACEENTITYCOMPONENT_HPP
#define FACEENTITYCOMPONENT_HPP

#include <anax/anax.hpp>
#include <LuaContext.hpp>

namespace ray {

using anax::Entity;

/**
 * An @c Entity with this @c Component will always face another @c Entity. If
 * the target @c Entity no longer exists, @c *this has no effect on its @c Entity.
 *
 * The target @c Entity must have a PositionComponent!
 */
struct FaceEntityComponent : public anax::Component<FaceEntityComponent>
{
    /**
     * Constructs a FaceEntityComponent that is not facing another @c Entity.
     */
    FaceEntityComponent() {}

    /**
     * Constructs a FaceEntityComponent that is facing a given @c Entity.
     *
     * @param e The @c Entity that @c *this's @c Entity will face.
     * @c e.isValid() must be true, and @c e must have a PositionComponent.
     */
    FaceEntityComponent(const Entity& e) : target(e) {}

    /**
     * The @c Entity that the @c Entity containing @c *this will face. If
     * @c target.isValid() is @c false, @c *this has no effect on its @c Entity.
     */
    Entity target;

    static void luaInit(LuaContext& lua) {
        lua.registerMember("target", &EntityFollowComponent::target);
    }
};

}
#endif // FACEENTITYCOMPONENT_HPP
