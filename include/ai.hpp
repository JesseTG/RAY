#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include <anax/anax.hpp>
#include <LuaContext.hpp>
#include "GameManager.hpp"

namespace ray {
namespace ai {
using anax::Entity;
using anax::ComponentFilter;

Entity seek(Entity e);
Entity wander(Entity e);
void initAIBindings(GameManager& game);
}
}

#endif // AI_HPP_INCLUDED
