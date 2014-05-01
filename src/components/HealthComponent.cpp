#include "HealthComponent.hpp"

#include <boost/optional.hpp>
#include "components.hpp"

namespace ray {
using boost::optional;

Entity HealthComponent::defaultDeath(Entity entity) {
    entity.addComponent<RemovalComponent>();
    return entity;
}

float HealthComponent::fraction() const {
    return this->health / float(this->max_health);
}

void HealthComponent::luaInit(LuaContext& lua) {

    lua.registerMember("health", &HealthComponent::health);
    lua.registerMember("onDeath", &HealthComponent::onDeath);
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
