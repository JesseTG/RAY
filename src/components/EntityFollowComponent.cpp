#include "EntityFollowComponent.hpp"

namespace ray {
void EntityFollowComponent::luaInit(LuaContext& lua) {
    lua.registerMember("target", &EntityFollowComponent::target);
}
}
