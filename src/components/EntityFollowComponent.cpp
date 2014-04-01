#include "EntityFollowComponent.hpp"

namespace ray {
using boost::optional;

void EntityFollowComponent::luaInit(LuaContext& lua) {
    lua.registerMember("target", &EntityFollowComponent::target);
    lua.writeFunction("EntityFollowComponent", "new",
    [](const optional<Entity&> e) {
        if (e) {
            return new EntityFollowComponent(*e);
        }
        else {
            return new EntityFollowComponent;
        }
    });
}
}
