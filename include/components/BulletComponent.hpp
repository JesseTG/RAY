#ifndef BULLETCOMPONENT_HPP
#define BULLETCOMPONENT_HPP

#include <anax/anax.hpp>


namespace ray {

/**
 * Lets an @c Entity be treated as a projectile weapon.
 *
 * If you want a bullet to travel until it hits a wall, set its lifetime to
 * positive infinity.
 */
struct BulletComponent : public anax::Component<BulletComponent>
{
    BulletComponent(double lifetime) : lifetime(lifetime) {}
    BulletComponent() : BulletComponent(0.0) {}

    double lifetime;
};
}

#endif // BULLETCOMPONENT_HPP
