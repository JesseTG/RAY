#ifndef TYPECOMPONENT_HPP
#define TYPECOMPONENT_HPP
#include <anax/anax.hpp>
#include <LuaContext.hpp>

namespace ray {
using anax::Entity;
struct TypeComponent : public anax::Component<TypeComponent>
{
    TypeComponent(int t);
    virtual ~TypeComponent();
    int type;

    static void luaInit(LuaContext& lua);
};

}

#endif // TYPECOMPONENT_HPP
