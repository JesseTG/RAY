#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <memory>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <unordered_map>

#include <Box2D/Box2D.h>
#include <anax/anax.hpp>
#include <LuaContext.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFGUI/SFGUI.hpp>

#include "fsm.hpp"
#include "managers.hpp"
#include "listeners.hpp"
#ifdef DEBUG
#include "DebugDrawing.hpp"
#endif

namespace ray {

using std::shared_ptr;
using std::make_shared;
using std::make_pair;
using std::pair;
using std::vector;
using std::string;
using std::map;
using std::unordered_map;
using sf::RenderWindow;
using sf::ContextSettings;
using sf::VideoMode;
using sf::Event;
using anax::World;

using util::WorldStateMachine;
using util::WorldState;
#ifdef DEBUG
using util::DebugDrawing;
#endif // DEBUG

class GameManager
{
    public:
        /**
        * The type used to key WorldStates.
        */
        typedef std::string WorldStateKey;

        /**
         * The type used to key transitions between WorldStates.
         */
        typedef std::string WorldStateTransition;

        typedef WorldStateMachine<WorldStateKey, WorldStateTransition, vector<Event>> GSM;

        GameManager();
        GameManager(GameManager& other) = delete;
        GameManager& operator=(GameManager& other) = delete;
        virtual ~GameManager();

        shared_ptr<LuaContext> getLuaContext() const;
        shared_ptr<b2World> getPhysicsWorld() const;
        shared_ptr<World> getWorld() const;
        shared_ptr<ScriptManager> getScriptManager() const;
        shared_ptr<ImageManager> getImageManager() const;
        shared_ptr<ShapeManager> getShapeManager() const;
        shared_ptr<RenderWindow> getRenderWindow() const;
        shared_ptr<SoundManager> getSoundManager() const;
        shared_ptr<MusicManager> getMusicManager() const;
        shared_ptr<LevelManager> getLevelManager() const;
        shared_ptr<sfg::SFGUI> getSfgui() const;
        shared_ptr<sfg::Desktop> getDesktop() const;
        shared_ptr<GSM> getStateMachine() const;
        anax::Entity getPlayer() const;

        void resetPhysicsWorld();
        void resetLuaContext();
        void resetWorld();
        void setPlayer(anax::Entity);
        int enemiesKilled;
    private:
        shared_ptr<LuaContext> _lua;
        shared_ptr<b2World> _physics_world;
        shared_ptr<World> _world;
        shared_ptr<ScriptManager> _script_manager;
        shared_ptr<ImageManager> _image_manager;
        shared_ptr<ShapeManager> _shape_manager;
        shared_ptr<RenderWindow> _render_window;
        shared_ptr<SoundManager> _sound_manager;
        shared_ptr<MusicManager> _music_manager;
        shared_ptr<LevelManager> _level_manager;
        shared_ptr<GSM> _state_machine;
        shared_ptr<sfg::SFGUI> _sfgui;
        shared_ptr<sfg::Desktop> _desktop;

        anax::Entity _player_entity;
        DestructionListener destruction_listener;
#ifdef DEBUG
        DebugDrawing _debug_draw;
#endif // DEBUG

        static const string SCRIPT_PATH;
        static const string SHAPE_PATH;
        static const string SOUND_PATH;
        static const string MUSIC_PATH;
        static const string LEVEL_PATH;
};
}

#endif // GAMEMANAGER_HPP
