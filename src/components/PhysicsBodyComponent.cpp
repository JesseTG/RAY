#include "PhysicsBodyComponent.hpp"

namespace ray {
void PhysicsBodyComponent::luaInit(LuaContext& lua) {
    lua.writeFunction(
        "PhysicsBodyComponent",
        "new",
    [](b2Body* body, Entity& e) {
        return new PhysicsBodyComponent(body, e);
    });
    lua.registerMember("body", &PhysicsBodyComponent::body);
    lua.registerMember("entity", &PhysicsBodyComponent::entity);
}

void PhysicsBodyComponent::_set_body(b2Body* b, Entity& e) {
    this->body = b;
    this->entity = e;
#ifdef DEBUG
    if (!this->body) {
        // If we got a null b2Body...
        throw std::invalid_argument("Expected a valid b2Body, got nullptr");
    }

    if (!e.isValid()) {
        // If the given Entity isn't properly attached to a World
        throw std::logic_error("Cannot use a PhysicsBodyComponent with an invalid Entity");
    }
#endif

    this->body->SetUserData(&(this->entity));
}
}
