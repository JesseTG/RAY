#include "GameManager.hpp"

namespace ray {

const string GameManager::SCRIPT_PATH = "data/script/scripts.json";
const string GameManager::SHAPE_PATH = "data/shape/shapes.json";
const string GameManager::SOUND_PATH = "data/sound/sounds.json";
const string GameManager::MUSIC_PATH = "data/music/music.json";

GameManager::GameManager() :
    _lua(new LuaContext),
    _physics_world(new b2World(b2Vec2_zero)),
    _world(new World),
    _script_manager(new ScriptManager(_lua, SCRIPT_PATH)),
    _image_manager(new ImageManager),
    _shape_manager(new ShapeManager(SHAPE_PATH)),
    _sound_manager(new SoundManager(SOUND_PATH)),
    _music_manager(new MusicManager(MUSIC_PATH)),
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

anax::Entity GameManager::getPlayer() const {
    return this->_player_entity;
}

void GameManager::resetPhysicsWorld() {
    this->_physics_world.reset(new b2World(b2Vec2_zero));
}

void GameManager::resetLuaContext() {
    this->_lua.reset(new LuaContext);
}

void GameManager::resetWorld() {
    this->_world.reset(new World);
}

void GameManager::setPlayer(anax::Entity e) {
    this->_player_entity = e;
}

}
