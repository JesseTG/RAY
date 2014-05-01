#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>
#include <memory>

namespace ray {

using std::weak_ptr;
using std::shared_ptr;
using anax::ComponentFilter;
using anax::Entity;

class PhysicsSystem : public anax::System<PhysicsSystem>
{
    public:
        PhysicsSystem(shared_ptr<b2World>);
        ~PhysicsSystem();
        void update();
        void setWorld(shared_ptr<b2World> world) {this->_world = world;}

        void onEntityAdded(Entity& entity) override;
        void onEntityRemoved(Entity& entity) override;
    private:
        weak_ptr<b2World> _world;

        static const ComponentFilter FILTER;
};
}

#endif // PHYSICSSYSTEM_HPP
