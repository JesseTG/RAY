#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>

namespace ray {

using anax::ComponentFilter;

class PhysicsSystem : public anax::System<PhysicsSystem>
{
    public:
        PhysicsSystem(b2World&);
        PhysicsSystem(b2World*);
        ~PhysicsSystem();
        void update();
        void setWorld(b2World* world) { this->_world = world;}
    private:
        b2World* _world;

        static const ComponentFilter FILTER;
};
}

#endif // PHYSICSSYSTEM_HPP
