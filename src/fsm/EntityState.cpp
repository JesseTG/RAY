#include "EntityState.hpp"


namespace util {
EntityState::EntityState(const Entity& e) :
    _entity(e),
    _components()
{
    //ctor
}

EntityState::~EntityState()
{
    //dtor
}
}
