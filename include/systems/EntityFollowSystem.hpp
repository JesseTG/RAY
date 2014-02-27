#ifndef ENTITYFOLLOWSYSTEM_HPP
#define ENTITYFOLLOWSYSTEM_HPP

#include <anax/anax.hpp>

namespace ray {

using anax::ComponentFilter;

class EntityFollowSystem : public anax::System<EntityFollowSystem>
{
    public:
        EntityFollowSystem();
        ~EntityFollowSystem();
        void update();
    private:
        static const ComponentFilter FILTER;
};
}

#endif // ENTITYFOLLOWSYSTEM_HPP
