#include "CollisionDamageListener.hpp"

#include <iostream>
#include <sstream>
#include <exception>
#include <cmath>

#include <anax/anax.hpp>

#include "components.hpp"

namespace ray {
using anax::Entity;

CollisionDamageListener::CollisionDamageListener()
{
    //ctor
}

CollisionDamageListener::~CollisionDamageListener()
{
    //dtor
}

void CollisionDamageListener::BeginContact(b2Contact* contact) {
    b2Fixture* a = contact->GetFixtureA();
    b2Fixture* b = contact->GetFixtureB();
    Entity* ea = static_cast<Entity*>(a->GetUserData());
    Entity* eb = static_cast<Entity*>(b->GetUserData());
    b2Body* ba = a->GetBody();
    b2Body* bb = b->GetBody();

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

    bool healtha = ea->hasComponent<HealthComponent>();
    bool healthb = eb->hasComponent<HealthComponent>();
    if (healtha && healthb) {
        // If both Entities have a notion of HP...
        const b2Vec2& va = ba->GetLinearVelocity();
        const b2Vec2& vb = bb->GetLinearVelocity();
        float va_mag = va.LengthSquared();
        float vb_mag = vb.LengthSquared();
        if (va_mag > vb_mag) {
            HealthComponent& health = eb->getComponent<HealthComponent>();
            int damage = ba->GetMass() * va_mag * 0.5;
            health.health -= damage;
            std::cout << "Body " << ba << " took " << damage << " damage" << std::endl;
        }
        else if (vb_mag > va_mag) {
            HealthComponent& health = ea->getComponent<HealthComponent>();
            int damage = bb->GetMass() * vb_mag * 0.5;
            health.health -= damage;
            std::cout << "Body " << bb << " took " << damage << " damage" << std::endl;
        }

    }
    else if (healtha) {
        // Else if only Entity A has health...

    }
    else if (healthb) {
        // Else if only Entity B has health...
    }
}

void CollisionDamageListener::EndContact(b2Contact* contact) {
}

}

