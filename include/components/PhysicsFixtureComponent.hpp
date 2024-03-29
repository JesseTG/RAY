#ifndef PHYSICSFIXTURECOMPONENT_HPP
#define PHYSICSFIXTURECOMPONENT_HPP

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>
#include <LuaContext.hpp>

namespace ray {
using anax::Entity;

/**
 * Entities with a PhysicsFixtureComponent are manipulated as @c b2Fixtures.
 * A @c b2Fixutre should never be on its own, i.e. it should always have a
 * parent @c b2Body.
 *
 * @note The underlying @c b2Fixture will have a pointer to an @c Entity stored
 * in its user data, to facilitate collision handling.
 * @note Upon destruction of *this, @c b2Body::DestroyBody(this->fixture) will
 * be called.
 *
 * @warning All Box2D objects will have their memory managed by Box2D itself.
 * Do @em not try to do it yourself.
 *
 * @see PhysicsBodyComponent For an @c Entity that represents a @c b2Body.
 * @see PhysicsSystem The primary @c System that operates on @c *this
 */
struct PhysicsFixtureComponent : public anax::Component<PhysicsFixtureComponent>
{
        /**
         * Constructs this PhysicsFixtureComponent with a given @c b2Body.
         *
         * Upon initialization, will call @c fixture->SetUserData(&e).
         * In debug builds, will ensure @c e.isValid() is @c true and that @c body
         * isn't @c nullptr.
         *
         * @param fixture Pointer to the @c b2Fixture that the owning @c Entity will
         * represent.
         * @param e The @c Entity that will hold @c *this. A pointer to it will be
         * stored in @c fixture's user data.
         */
        PhysicsFixtureComponent(b2Fixture* fixture, Entity& e);

        /**
         * Destructor.
         */
        virtual ~PhysicsFixtureComponent();

        /**
         * Pointer to a @c b2Fixture that was created by some @c b2Body.
         */
        b2Fixture* fixture;

        Entity entity;

        static void luaInit(LuaContext& lua);

    private:
        void _set_fixture(b2Fixture* fixture, Entity& e);
};
}

#endif // PHYSICSFIXTURECOMPONENT_HPP

