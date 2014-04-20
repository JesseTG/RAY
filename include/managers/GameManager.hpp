#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <memory>

#include <Box2D/Box2D.h>
#include <anax/anax.hpp>
#include <LuaContext.hpp>

#include "components.hpp"
#include "entities.hpp"
#include "config.hpp"
#include "managers.hpp"
#include <SFGUI/SFGUI.hpp>

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
        shared_ptr<ScriptManager> getScriptManager() const;
        shared_ptr<ImageManager> getImageManager() const;
        shared_ptr<ShapeManager> getShapeManager() const;
        shared_ptr<sfg::SFGUI> getSfgui() const;
        shared_ptr<sfg::Desktop> getDesktop() const;
        sfg::ProgressBar::Ptr getHealthBar() const;
        anax::Entity getPlayer() const;

        void resetPhysicsWorld();
        void resetLuaContext();
        void resetWorld();
        void setPlayer(anax::Entity);
    protected:
    private:
        shared_ptr<LuaContext> _lua;
        shared_ptr<b2World> _physics_world;
        shared_ptr<World> _world;
        shared_ptr<ScriptManager> _script_manager;
        shared_ptr<ImageManager> _image_manager;
        shared_ptr<ShapeManager> _shape_manager;
        shared_ptr<sfg::SFGUI> _sfgui;
        shared_ptr<sfg::Desktop> _desktop;
        sfg::ProgressBar::Ptr _health_bar;
        anax::Entity _player_entity;
};
}

#endif // GAMEMANAGER_HPP
