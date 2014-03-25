#include "GameManager.hpp"

namespace ray {
GameManager::GameManager() :
    _lua(new LuaContext),
    _physics_world(new b2World(b2Vec2(0, 0))),
    _world(new World)
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

}
