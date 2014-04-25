#include "entities.hpp"
#include "managers.hpp"
#include "util.hpp"

#include <memory>

#include <SFML/System.hpp>
#include <LuaContext.hpp>

namespace ray {
namespace entities {
using std::dynamic_pointer_cast;
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
            lua.registerMember("group", &GameShape::group);
            lua.registerFunction<GameShape, Vector2f(int)>("startPosition",
            [](GameShape& shape, int index) {
                return dynamic_pointer_cast<sf::Transformable>(shape.graphics_shapes[index - 1])->getPosition();
            });
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
