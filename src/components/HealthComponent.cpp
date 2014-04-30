#include "HealthComponent.hpp"

#include <boost/optional.hpp>

namespace ray {
using boost::optional;

float HealthComponent::fraction() const {
    return this->health / float(this->max_health);
}
void HealthComponent::luaInit(LuaContext& lua) {

    lua.registerMember("health", &HealthComponent::health);

    lua.writeFunction("HealthComponent", "new",
    [](const optional<int>& health) {
        if (health) {
            return new HealthComponent(*health);
        }
        else {
            return new HealthComponent;
        }
    });
}
}
