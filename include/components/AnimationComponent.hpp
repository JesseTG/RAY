#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include <string>

#include <anax/anax.hpp>
#include <LuaContext.hpp>
#include <Thor/Animation.hpp>


namespace ray {
using std::string;
using anax::Entity;
using thor::Animator;

struct AnimationComponent : public anax::Component<AnimationComponent>
{
    typedef Animator<Entity, string> EntityAnimator;

    EntityAnimator animator;


    static void luaInit(LuaContext& lua);
};
}

#endif // ANIMATIONCOMPONENT_HPP
