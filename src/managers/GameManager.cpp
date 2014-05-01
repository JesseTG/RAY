#include "GameManager.hpp"

#include "config.hpp"
#include "states.hpp"

namespace ray {

const string GameManager::SCRIPT_PATH = "data/script/scripts.json";
const string GameManager::SHAPE_PATH = "data/shape/shapes.json";
const string GameManager::SOUND_PATH = "data/sound/sounds.json";
const string GameManager::MUSIC_PATH = "data/music/music.json";
const string GameManager::LEVEL_PATH = "data/level/levels.json";

GameManager::GameManager() :
    _lua(new LuaContext),
    _world(new World),
    _script_manager(new ScriptManager(_lua, SCRIPT_PATH)),
    _image_manager(new ImageManager),
    _shape_manager(new ShapeManager(SHAPE_PATH)),
    _sound_manager(new SoundManager(SOUND_PATH)),
    _music_manager(new MusicManager(MUSIC_PATH)),
    _level_manager(new LevelManager(LEVEL_PATH)),
    _physics_world(new b2World(b2Vec2_zero)),
    _sfgui(new sfg::SFGUI),
    _desktop(new sfg::Desktop)
{
    this->_render_window = make_shared<RenderWindow>(
                               VideoMode(SCREEN_SIZE.x, SCREEN_SIZE.y),
                               "RAY",
                               sf::Style::Default,
                               ContextSettings(0, 0, 4)
                           );
    this->_render_window->setFramerateLimit(FPS);
#ifdef DEBUG
    this->_debug_draw.setWindow(*this->_render_window);
    this->resetPhysicsWorld();
#endif // DEBUG
    this->_state_machine.reset(
        new GSM(
            *this->_world,
            "start",
    {
        {"start",  make_shared<TitleScreenState>(*this)},
        {"game", make_shared<InGameState>(*this)},
    },
    {
        {make_pair("swap", "start"), "game"},
        {make_pair("swap", "game"), "start"},
        {make_pair("advance", "game"), "game"},
    }));

}

GameManager::~GameManager()
{
    //dtor
}

shared_ptr<LuaContext> GameManager::getLuaContext() const {
    return this->_lua;
}

shared_ptr<b2World> GameManager::getPhysicsWorld() const {
    return this->_physics_world;
}

shared_ptr<World> GameManager::getWorld() const {
    return this->_world;
}

shared_ptr<ScriptManager> GameManager::getScriptManager() const {
    return this->_script_manager;
}

shared_ptr<ImageManager> GameManager::getImageManager() const {
    return this->_image_manager;
}

shared_ptr<ShapeManager> GameManager::getShapeManager() const {
    return this->_shape_manager;
}

shared_ptr<RenderWindow> GameManager::getRenderWindow() const {
    return this->_render_window;
}

shared_ptr<SoundManager> GameManager::getSoundManager() const {
    return this->_sound_manager;
}

shared_ptr<MusicManager> GameManager::getMusicManager() const {
    return this->_music_manager;
}

shared_ptr<sfg::SFGUI> GameManager::getSfgui() const {
    return this->_sfgui;
}

shared_ptr<sfg::Desktop> GameManager::getDesktop() const {
    return this->_desktop;
}

shared_ptr<GameManager::GSM> GameManager::getStateMachine() const {
    return this->_state_machine;
}

shared_ptr<LevelManager> GameManager::getLevelManager() const {
    return this->_level_manager;
}

anax::Entity GameManager::getPlayer() const {
    return this->_player_entity;
}

void GameManager::resetPhysicsWorld() {
    for (b2Body* b = this->_physics_world->GetBodyList(); b; b = b->GetNext()) {
        this->_physics_world->DestroyBody(b);
    }

    this->_physics_world->SetDestructionListener(&this->destruction_listener);
#ifdef DEBUG
    this->_physics_world->SetDebugDraw(&(this->_debug_draw));
#endif // DEBUG
}

void GameManager::resetLuaContext() {
    this->_lua.reset(new LuaContext);
}

void GameManager::resetWorld() {
    auto ent = this->_world->getEntities();
    for (Entity& e : ent) {
        e.deactivate();
    }
    this->_world->removeAllSystems();
    // Can't call w.clear(), it causes a segfault if you go back to this
    // state later; a bug in Anax?
}

void GameManager::setPlayer(anax::Entity e) {
    this->_player_entity = e;
}

}
