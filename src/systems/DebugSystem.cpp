#include "DebugSystem.hpp"

namespace ray {
DebugSystem::DebugSystem(RenderWindow& window, b2World& world, lua_State& lua) :
    _window(&window),
    _physics_world(&world),
    _lua(&lua),
    Base()
{
    //ctor
}

DebugSystem::~DebugSystem()
{
    //dtor
}

#ifdef DEBUG
void DebugSystem::update(const vector<Event>& events) {
}
#else
void DebugSystem::update(const vector<Event>& events) {
    // Do nothing in Release mode
}
#endif // DEBUG

}
