#include "entities.hpp"
#include "managers.hpp"
#include "util.hpp"

#include <LuaContext.hpp>

namespace ray {
namespace entities {
void initManagerTypeBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();
    lua.writeVariable("Resource", LuaEmptyArray);
    {
        lua.writeVariable("Resource", "Shape", LuaEmptyArray);
        {
            lua.writeFunction("Resource", "Shape", "Get", [&game](const string& id) {
                return game.getShapeManager()->getShape(id);
            });
            lua.registerMember("graphics_shapes", &GameShape::graphics_shapes);
            lua.registerMember("physics_shapes", &GameShape::physics_shapes);
        }

        lua.writeVariable("Resource", "Sound", LuaEmptyArray);
        {
        }

        lua.writeVariable("Resource", "Music", LuaEmptyArray);
        {
        }

        lua.writeVariable("Resource", "Image", LuaEmptyArray);
        {
        }
    }
}
}

}
