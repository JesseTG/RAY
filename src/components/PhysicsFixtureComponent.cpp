#include "PhysicsFixtureComponent.hpp"

#include <iostream>

namespace ray {

PhysicsFixtureComponent::PhysicsFixtureComponent(b2Fixture* fixture, Entity& e) {
    this->_set_fixture(fixture, e);
}

PhysicsFixtureComponent::~PhysicsFixtureComponent() {
    this->fixture->SetUserData(nullptr);
}

void PhysicsFixtureComponent::_set_fixture(b2Fixture* fixture, Entity& e) {
    this->fixture = fixture;
    this->entity = e;

#ifdef DEBUG
    if (!this->fixture) {
        // If we got a null b2Fixture...
        throw std::invalid_argument("Expected a valid b2Fixture, got nullptr");
    }

    if (!e.isValid()) {
        // If the given Entity isn't properly attached to a World
        throw std::logic_error("Cannot use a PhysicsFixtureComponent with an invalid Entity");
    }
#endif

    this->fixture->SetUserData(&(this->entity));
}
void PhysicsFixtureComponent::luaInit(LuaContext& lua) {
    lua.registerMember("fixture", &PhysicsFixtureComponent::fixture);
    lua.writeFunction(
        "PhysicsFixtureComponent",
        "new",
    [](b2Fixture* fixture, Entity& e) {
        return new PhysicsFixtureComponent(fixture, e);
    });
}
}

