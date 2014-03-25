#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <memory>

#include <Box2D/Box2D.h>
#include <anax/anax.hpp>
#include <LuaContext.hpp>

#include "components.hpp"
#include "entities.hpp"
#include "config.hpp"

namespace ray {
    using std::shared_ptr;
    using std::make_shared;
using anax::World;
class GameManager
{
    public:
        GameManager();
        virtual ~GameManager();

        shared_ptr<LuaContext> getLuaContext() const;
        shared_ptr<b2World> getPhysicsWorld() const;
        shared_ptr<World> getWorld() const;

        void resetPhysicsWorld() {
            this->_physics_world.reset(new b2World(b2Vec2_zero));
        }
    protected:
    private:
        shared_ptr<LuaContext> _lua;
        shared_ptr<b2World> _physics_world;
        shared_ptr<World> _world;
};
}

#endif // GAMEMANAGER_HPP
