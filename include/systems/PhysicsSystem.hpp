#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>

namespace ray {
class PhysicsSystem : public anax::System<PhysicsSystem>
{
    public:
        PhysicsSystem();
        ~PhysicsSystem();
        void update();
    private:
        b2World _world;
};
}

#endif // PHYSICSSYSTEM_HPP
