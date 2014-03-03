#include "DebugSystem.hpp"

namespace ray {
DebugSystem::DebugSystem(RenderWindow& window, b2World& world) :
    _window(&window),
    _physics_world(&world)
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
