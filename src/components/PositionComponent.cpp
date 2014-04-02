#include "PositionComponent.hpp"

namespace ray {

void PositionComponent::luaInit(LuaContext& lua) {
    lua.writeFunction(
        "PositionComponent",
        "new",
        [](const variant<Vector2f, b2Vec2, float>& arg1,
           const optional<float> arg2)
    {

        if (arg2) {
            // If the scripter passed in two arguments...
            const float* fptr = get<float>(&arg1);
            if (fptr) {
                // If the first argument was a float...
                return new PositionComponent(*fptr, *arg2);
            }
            else {
                throw new invalid_argument(
                    "Expected a float for argument 1, got a " + string(arg1.type().name())
                );
            }
        }
        else {
            // If the scripter passed in one argument...
            const Vector2f* sfvecptr = get<Vector2f>(&arg1);
            const b2Vec2* b2vecptr = get<b2Vec2>(&arg1);
            if (sfvecptr) {
                // If the first argument was a sf::Vector...
                return new PositionComponent(*sfvecptr);
            }
            else if (b2vecptr) {
                // If the first argument was a b2Vec2...
                return new PositionComponent(*b2vecptr);
            }
            else {
                // Hm, the user passed in a single float, that won't do
                throw invalid_argument(
                    "Expected a sf::Vector2 or a b2Vec2, got a " + string(arg1.type().name())
                );
            }
        }
    });
    lua.registerMember("position", &PositionComponent::position);
    lua.registerMember<PositionComponent, float>("x",
    [](const PositionComponent& p) {
        return p.position.x;
    },
    [](PositionComponent& p, const float val) {
        p.position.x = val;
    });

    lua.registerMember<PositionComponent, float>("y",
    [](const PositionComponent& p) {
        return p.position.y;
    },
    [](PositionComponent& p, const float val) {
        p.position.y = val;
    });
}

}
