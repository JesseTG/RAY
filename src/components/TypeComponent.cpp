#include "TypeComponent.hpp"

namespace ray {
TypeComponent::TypeComponent(int t) : type(t)
{
    //ctor
}

TypeComponent::~TypeComponent()
{
    //dtor
}

void TypeComponent::luaInit(LuaContext& lua) {
    lua.writeFunction(
        "TypeComponent",
        "new",
    [](const int type) {
        return new TypeComponent(type);
    });

    lua.registerMember("type", &TypeComponent::type);
}

}
