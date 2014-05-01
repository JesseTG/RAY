#include "TractorBeamRepellingListener.hpp"

#include <iostream>
#include <sstream>
#include <exception>

#include "components.hpp"

namespace ray {
TractorBeamRepellingListener::TractorBeamRepellingListener()
{
    //ctor
    // Scriptable component: called once when a flag is set?
}

TractorBeamRepellingListener::~TractorBeamRepellingListener()
{
    this->_grips.clear();
}

void TractorBeamRepellingListener::clearGrips() {
    this->_grips.clear();
}

unordered_multimap<b2Fixture*, b2Fixture*>& TractorBeamRepellingListener::getTractorBeamGrips() {
    return this->_grips;
}

void TractorBeamRepellingListener::BeginContact(b2Contact* contact) {
    b2Fixture* a = contact->GetFixtureA();
    b2Fixture* b = contact->GetFixtureB();
    Entity* ea = static_cast<Entity*>(a->GetUserData());
    Entity* eb = static_cast<Entity*>(b->GetUserData());

    Entity* beam = nullptr;
    Entity* target = nullptr;
    b2Fixture* beamfix = nullptr;
    b2Fixture* targetfix = nullptr;

#ifdef DEBUG
    if (!ea) {
        // If Fixture A doesn't have an Entity attached to it...
        std::stringstream err;
        err << "b2Fixture A at " << a << " has no owning Entity";
        throw std::logic_error(err.str());
    }

    if (!eb) {
        // If Fixture B doesn't have an attached Entity...
        std::stringstream err;
        err << "b2Fixture B at " << b << " has no owning Entity";
        throw std::logic_error(err.str());
    }
#endif // DEBUG

    if (!(ea->isValid() && eb->isValid())) return;

    bool tracta = ea->hasComponent<TractorBeamComponent>();
    bool tractb = eb->hasComponent<TractorBeamComponent>();
    if (tracta == tractb) {
        // If both or neither entities are tractor beams...
        // (Tractor beams can't move each other, and non-beam collisions are
        // irrelevant in this listener)
        return;
    }
    else if (tracta) {
        // If Fixture A's entity is a tractor beam...
        beam = ea;
        beamfix = a;
        target = eb;
        targetfix = b;
    }
    else if (tractb) {
        // Else if Fixture B's entity is a tractor beam...
        beam = eb;
        beamfix = b;
        target = ea;
        targetfix = a;
    }

    if (target->hasComponent<TractorBeamRepellableComponent>()) {
        this->_grips.insert(make_pair(beamfix, targetfix));
    }
}

void TractorBeamRepellingListener::EndContact(b2Contact* contact) {
    this->_grips.erase(contact->GetFixtureA());
    this->_grips.erase(contact->GetFixtureB());
}

}
