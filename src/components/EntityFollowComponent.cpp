#include "EntityFollowComponent.hpp"

namespace ray {
using boost::optional;

void EntityFollowComponent::luaInit(LuaContext& lua) {
    lua.registerMember("target", &EntityFollowComponent::target);
    lua.registerMember("maxSpeed", &EntityFollowComponent::maxSpeed);
    lua.writeFunction("EntityFollowComponent", "new",
    [](const optional<Entity&> e, const optional<float> maxSpeed) {
        if (e && maxSpeed) {
            return new EntityFollowComponent(*e, *maxSpeed);
        }
        else if (e) {
            return new EntityFollowComponent(*e);
        }
        else {
            return new EntityFollowComponent;
        }
    });
}
}
