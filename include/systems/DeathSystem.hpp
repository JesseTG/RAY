#ifndef DEATHSYSTEM_HPP
#define DEATHSYSTEM_HPP

#include <anax/anax.hpp>
#include "components.hpp"
#include "managers/GameManager.hpp"

namespace ray {

using anax::Entity;
using anax::ComponentFilter;
class DeathSystem : public anax::System<DeathSystem>
{
    public:
        DeathSystem();
        DeathSystem(GameManager *);
        virtual ~DeathSystem();
        void update();
        GameManager *gm;
    private:
        static const ComponentFilter FILTER;
};
}

#endif // DEATHSYSTEM_HPP
