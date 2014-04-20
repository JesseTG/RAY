#include "HealthComponent.hpp"

namespace ray {
void HealthComponent::luaInit(LuaContext& lua) {
    lua.registerMember("health", &HealthComponent::health);
    lua.registerMember("armor", &HealthComponent::armor);
    lua.writeFunction("HealthComponent", "new",
    [](const int initialHealth, const int initialArmor) {
        return new HealthComponent(initialHealth, initialArmor);
    });
}
}
