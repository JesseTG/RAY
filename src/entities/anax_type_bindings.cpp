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

void initAnaxTypeBindings() {
    _lua->writeVariable("Anax", LuaEmptyArray);
    {
        _lua->writeVariable("Anax", "world", _world);
        {
            _lua->registerFunction("killEntity", &World::killEntity);
            _lua->registerFunction("killEntities", &World::killEntities);
            _lua->registerFunction("activateEntity", &World::activateEntity);
            _lua->registerFunction("deactivateEntity", &World::deactivateEntity);
            _lua->registerFunction("isActivated", &World::isActivated);
            _lua->registerFunction("isValid", &World::isValid);
            _lua->registerFunction("refresh", &World::refresh);
            _lua->registerFunction("clear", &World::clear);
            _lua->registerFunction("getEntityCount", &World::getEntityCount);
        }

        _lua->writeVariable("Anax", "Entity", LuaEmptyArray);
        {
            _lua->writeFunction("Anax", "Entity", "new", []() {
                Entity e = _world->createEntity();
                _world->activateEntity(e);
                return e;
            });
            _lua->registerFunction("isActivated", &Entity::isActivated);
            _lua->registerFunction("activate", &Entity::activate);
            _lua->registerFunction("deactivate", &Entity::deactivate);
            _lua->registerFunction("isValid", &Entity::isValid);
        }
    }
}
}
}
