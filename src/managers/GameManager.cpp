#include "GameManager.hpp"

namespace ray {
GameManager::GameManager() :
    _lua(new LuaContext),
    _physics_world(new b2World(b2Vec2_zero)),
    _world(new World),
    _script_manager(new ScriptManager(_lua)),
    _image_manager(new ImageManager),
    _shape_manager(new ShapeManager)
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

void GameManager::resetPhysicsWorld() {
    this->_physics_world.reset(new b2World(b2Vec2_zero));
}

void GameManager::resetLuaContext() {
    this->_lua.reset(new LuaContext);
}

void GameManager::resetWorld() {
    this->_world.reset(new World);
}

}
