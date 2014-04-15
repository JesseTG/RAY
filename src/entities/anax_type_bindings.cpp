#include "entities.hpp"
#include "util.hpp"

#include <LuaContext.hpp>

namespace ray {
namespace entities {
using boost::get;
using boost::optional;
using boost::variant;

using anax::Entity;
using anax::World;

void initAnaxTypeBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();
    lua.writeVariable("Anax", LuaEmptyArray);
    {
        lua.writeVariable("Anax", "World", game.getWorld());
        {
            lua.registerFunction("killEntity", &World::killEntity);
            lua.registerFunction("killEntities", &World::killEntities);
            lua.registerFunction("activateEntity", &World::activateEntity);
            lua.registerFunction("deactivateEntity", &World::deactivateEntity);
            lua.registerFunction("isActivated", &World::isActivated);
            lua.registerFunction("isValid", &World::isValid);
            lua.registerFunction("refresh", &World::refresh);
            lua.registerFunction("clear", &World::clear);
            lua.registerFunction("getEntityCount", &World::getEntityCount);
        }

        lua.writeVariable("Anax", "Entity", LuaEmptyArray);
        {
            lua.writeFunction("Anax", "Entity", "new", [&game]() {
                Entity e = game.getWorld()->createEntity();
                game.getWorld()->activateEntity(e);
                return e;
            });
            lua.registerFunction("isActivated", &Entity::isActivated);
            lua.registerFunction("activate", &Entity::activate);
            lua.registerFunction("deactivate", &Entity::deactivate);
            lua.registerFunction("isValid", &Entity::isValid);
        }
    }
}
}
}
