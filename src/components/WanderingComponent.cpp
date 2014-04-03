#include "WanderingComponent.hpp"

namespace ray {
void WanderingComponent::luaInit(LuaContext& lua) {
    lua.registerMember("timer", &WanderingComponent::timer);
    lua.registerMember("speed", &WanderingComponent::speed);
    lua.registerMember("angle", &WanderingComponent::angle);
}
}
