#include "PhysicsFixtureComponent.hpp"

namespace ray {
void PhysicsFixtureComponent::luaInit(LuaContext& lua) {
    lua.writeFunction(
        "PhysicsFixtureComponent",
        "new",
    [](b2Fixture* fixture, Entity& e) {
        return new PhysicsFixtureComponent(fixture, e);
    });
}
}
