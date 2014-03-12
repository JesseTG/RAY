#include "FaceEntityComponent.hpp"

namespace ray {
FaceEntityComponent::FaceEntityComponent(const Entity& e) : target(e) {
}

void FaceEntityComponent::luaInit(LuaContext& lua) {
    lua.registerMember("target", &FaceEntityComponent::target);
}
}
