#include "FourWayControlComponent.hpp"

namespace ray {
typedef Keyboard::Key Key;

FourWayControlComponent::FourWayControlComponent() :
    FourWayControlComponent::FourWayControlComponent(Key::W, Key::S, Key::A, Key::D) {
}

FourWayControlComponent::FourWayControlComponent(
    const Key up,
    const Key down,
    const Key left,
    const Key right
) :
    up(up),
    down(down),
    left(left),
    right(right) {
}

void FourWayControlComponent::luaInit(LuaContext& lua) {
    lua.registerMember("up", &FourWayControlComponent::up);
    lua.registerMember("down", &FourWayControlComponent::down);
    lua.registerMember("left", &FourWayControlComponent::left);
    lua.registerMember("right", &FourWayControlComponent::right);
}
}
