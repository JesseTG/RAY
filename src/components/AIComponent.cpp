#include "AIComponent.hpp"

namespace ray {
AIComponent::AIComponent(
    const AIUpdateFunction& ai
) :
    update(ai)
{
    isMovingRight = false;
    isMovingDown = false;
    moveStepsX = 0;
    moveStepsY = 0;
    isStationary = true;
}

void AIComponent::luaInit(LuaContext& lua) {

    lua.writeFunction("AIComponent", "new",
    [](const AIUpdateFunction& ai) {
        return new AIComponent(ai);
    });

    lua.registerMember("update", &AIComponent::update);
}

}
