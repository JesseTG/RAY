#include "HealthComponent.hpp"

namespace ray {
void HealthComponent::luaInit(LuaContext& lua) {
    lua.registerMember("health", &HealthComponent::health);
    lua.registerMember<HealthComponent, float>("health",
    [](const HealthComponent& h) {
        return h.health;
    },
    [](HealthComponent& h, float val) {
        h.health = val;
    });
}
}
