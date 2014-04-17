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
        angle(0),
        active(false)
    {}

    float timer;
    float speed;
    float angle;
    bool active;

    static void luaInit(LuaContext& lua);
    bool getActive();
};
}

#endif // WANDERINGCOMPONENT_HPP
