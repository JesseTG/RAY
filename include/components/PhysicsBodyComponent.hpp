#ifndef PHYSICSBODYCOMPONENT_HPP
#define PHYSICSBODYCOMPONENT_HPP

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>
#include <LuaContext.hpp>

namespace ray {

using anax::Entity;

/**
 * Entities with a PhysicsBodyComponent are manipulated as @c b2Bodys. Remember
 * that not all visible collidable entities are bodies, just ones that aren't
 * usually attached to other ones. Anything not part of the gameplay (e.g. GUI
 * elements, background items, etc.) should NOT have this component.
 *
 * @note The underlying @c b2Body will have a pointer to an @c Entity stored in its
 * user data, to facilitate collision handling.
 * @note Upon destruction of *this, @c b2World::DestroyBody(this->body) will be
 * called.
 *
 * @warning All Box2D objects will have their memory managed by Box2D itself.
 * Do @em not try to do it yourself.
 *
 * @see PhysicsFixtureComponent For an @c Entity that represents a @c b2Fixture.
 * @see PhysicsSystem The primary @c System that operates on @c *this
 */
struct PhysicsBodyComponent : public anax::Component<PhysicsBodyComponent>
{
        /**
         * Constructs this PhysicsBodyComponent with a given @c b2Body.
         *
         * Upon initialization, will call @c body->SetUserData(&e).
         * In debug builds, will ensure @c e.isValid() is @c true and that @c body
         * isn't @c nullptr.
         *
         * @param body Pointer to the @c b2Body that the owning @c Entity will
         * represent.
         * @param e The @c Entity that will hold @c *this. A pointer to it will be
         * stored in @c body's user data.
         */
        PhysicsBodyComponent(b2Body* body, Entity& e);

        /**
         * Destructor.
         */
        ~PhysicsBodyComponent();

        /**
         * Pointer to a @c b2Body that was created by the game's @c b2World. Not a
         * smart pointer because Box2D objects are managed in a special way that a
         * smart pointer would fuck up.
         */
        b2Body* body;

        Entity entity;

        static void luaInit(LuaContext& lua);

    private:
        void _set_body(b2Body* b, Entity& e);
};

}
#endif // PHYSICSBODYCOMPONENT_HPP
