#include "GameManager.hpp"

namespace ray {
GameManager::GameManager() :
    _lua(new LuaContext),
    _physics_world(new b2World(b2Vec2_zero)),
    _world(new World),
    _script_manager(new ScriptManager(_lua)),
    _image_manager(new ImageManager),
    _shape_manager(new ShapeManager),
    _sfgui(new sfg::SFGUI),
    _desktop(new sfg::Desktop),
    _health_bar(sfg::ProgressBar::Create())
{
    //ctor
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

shared_ptr<sfg::SFGUI> GameManager::getSfgui() const {
    return this->_sfgui;
}

shared_ptr<sfg::Desktop> GameManager::getDesktop() const {
    return this->_desktop;
}

sfg::ProgressBar::Ptr GameManager::getHealthBar() const {
    return this->_health_bar;
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
