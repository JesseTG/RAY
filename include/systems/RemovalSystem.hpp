#ifndef REMOVALSYSTEM_HPP
#define REMOVALSYSTEM_HPP

#include <anax/anax.hpp>


namespace ray {
using anax::System;
using anax::Entity;
using anax::ComponentFilter;
class RemovalSystem : public System<RemovalSystem>
{
    public:
        RemovalSystem();
        virtual ~RemovalSystem();
        void update();
    private:
        void onEntityAdded(Entity& entity) override;
        void onEntityRemoved(Entity& entity) override;
        static const ComponentFilter FILTER;
};
}

#endif // REMOVALSYSTEM_HPP
