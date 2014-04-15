#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <memory>

#include <Box2D/Box2D.h>
#include <anax/anax.hpp>
#include <LuaContext.hpp>
#include <SFML/Graphics.hpp>

#include "components.hpp"
#include "entities.hpp"
#include "config.hpp"
#include "managers.hpp"

namespace ray {

using std::shared_ptr;
using std::make_shared;
using sf::RenderWindow;
using sf::ContextSettings;
using sf::VideoMode;

using anax::World;

class GameManager
{
    public:
        GameManager();
        GameManager(GameManager& other) = delete;
        virtual ~GameManager();

        shared_ptr<LuaContext> getLuaContext() const;
        shared_ptr<b2World> getPhysicsWorld() const;
        shared_ptr<World> getWorld() const;
        shared_ptr<ScriptManager> getScriptManager() const;
        shared_ptr<ImageManager> getImageManager() const;
        shared_ptr<ShapeManager> getShapeManager() const;
        shared_ptr<RenderWindow> getRenderWindow() const;

        void resetPhysicsWorld();
        void resetLuaContext();
        void resetWorld();
    protected:
    private:
        shared_ptr<LuaContext> _lua;
        shared_ptr<b2World> _physics_world;
        shared_ptr<World> _world;
        shared_ptr<ScriptManager> _script_manager;
        shared_ptr<ImageManager> _image_manager;
        shared_ptr<ShapeManager> _shape_manager;
        shared_ptr<RenderWindow> _render_window;
};
}

#endif // GAMEMANAGER_HPP
