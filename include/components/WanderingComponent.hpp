#ifndef WANDERINGCOMPONENT_HPP
#define WANDERINGCOMPONENT_HPP

#include <anax/anax.hpp>
#include <LuaContext.hpp>

namespace ray{

struct WanderingComponent : public anax::Component<WanderingComponent>
{
    WanderingComponent() :
        timer(2),
        speed(0),
        angle(0)
    {}

    float timer;
    float speed;
    float angle;

    static void luaInit(LuaContext& lua);
};
}

#endif // WANDERINGCOMPONENT_HPP
