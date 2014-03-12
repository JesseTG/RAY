#include "TractorBeamComponent.hpp"

namespace ray {
void TractorBeamComponent::luaInit(LuaContext& lua) {
    lua.registerMember("starting_width", &TractorBeamComponent::starting_width);
    lua.registerMember("starting_angle", &TractorBeamComponent::starting_angle);
    lua.registerMember("length", &TractorBeamComponent::length);
    lua.registerMember("force", &TractorBeamComponent::force);
}
}
