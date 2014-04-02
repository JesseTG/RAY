#include "VelocityComponent.hpp"

namespace ray {
void VelocityComponent::luaInit(LuaContext& lua) {
    lua.registerMember("velocity", &VelocityComponent::velocity);
    lua.registerMember<VelocityComponent, float>("x",
    [](const VelocityComponent& v) -> float {
        return v.velocity.x;
    },
    [](VelocityComponent& v, float val) {
        v.velocity.x = val;
    });

    lua.registerMember<VelocityComponent, float>("y",
    [](const VelocityComponent& v) -> float {
        return v.velocity.y;
    },
    [](VelocityComponent& v, float val) {
        v.velocity.y = val;
    });
}
}
