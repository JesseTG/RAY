#include "MultiContactListener.hpp"

#include <utility>

namespace ray {
    using std::move;
MultiContactListener::MultiContactListener()
{
    //ctor
}

MultiContactListener::~MultiContactListener()
{
    //dtor
}

void MultiContactListener::addListener(b2ContactListener* listener) {
    this->_listeners.emplace_back(listener);
}

void MultiContactListener::clearListeners() {
    this->_listeners.clear();
}

void MultiContactListener::BeginContact(b2Contact* contact) {
    for (auto& ptr : this->_listeners) {
        ptr->BeginContact(contact);
    }
}

void MultiContactListener::EndContact(b2Contact* contact) {
    for (auto& ptr : this->_listeners) {
        ptr->EndContact(contact);
    }
}

void MultiContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    for (auto& ptr : this->_listeners) {
        ptr->PreSolve(contact, oldManifold);
    }
}

void MultiContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    for (auto& ptr : this->_listeners) {
        ptr->PostSolve(contact, impulse);
    }
}

}
