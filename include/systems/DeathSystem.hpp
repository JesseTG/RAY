#ifndef DEATHSYSTEM_HPP
#define DEATHSYSTEM_HPP

#include <anax/anax.hpp>
#include "components.hpp"

namespace ray {

using anax::Entity;
using anax::ComponentFilter;
class DeathSystem : public anax::System<DeathSystem>
{
    public:
        DeathSystem();
        virtual ~DeathSystem();
        void update();
    private:
        static const ComponentFilter FILTER;
};
}

#endif // DEATHSYSTEM_HPP
