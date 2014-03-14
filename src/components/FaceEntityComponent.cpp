#include "FaceEntityComponent.hpp"

#include <boost/optional.hpp>

namespace ray {
using boost::optional;

FaceEntityComponent::FaceEntityComponent(const Entity& e) : target(e) {
}

void FaceEntityComponent::luaInit(LuaContext& lua) {
    lua.registerMember("target", &FaceEntityComponent::target);
    lua.writeFunction("FaceEntityComponent", "new",
    [](const optional<Entity&> e) {
        if (e) {
            return new FaceEntityComponent(*e);
        }
        else {
            return new FaceEntityComponent;
        }
    });
}
}
