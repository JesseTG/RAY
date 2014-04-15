#include "AIComponent.hpp"

namespace ray {
AIComponent::AIComponent(
    const AIUpdateFunction& ai
) :
    update(ai)
{
    //ctor
}

void AIComponent::luaInit(LuaContext& lua) {

    lua.writeFunction("AIComponent", "new",
    [](const AIUpdateFunction& ai) {
        return new AIComponent(ai);
    });

    lua.registerMember("update", &AIComponent::update);
}

}
