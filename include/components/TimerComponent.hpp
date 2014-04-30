#ifndef TIMERCOMPONENT_HPP
#define TIMERCOMPONENT_HPP

#include <anax/anax.hpp>
#include <Thor/Time.hpp>
#include <SFML/System.hpp>
#include <LuaContext.hpp>

namespace ray {
using anax::Component;
using anax::Entity;
using thor::CallbackTimer;
using sf::Time;
struct TimerComponent : public Component<TimerComponent>
{
    TimerComponent(const Time& time, const bool loop=true)
        : loop(loop), startTime(time) {
        timer.reset(time);
    }

    template<class NumberType>
    TimerComponent(const NumberType seconds, const bool loop=true)
        : loop(loop), startTime(sf::seconds(seconds)) {
        timer.reset(this->startTime);
    }

    TimerComponent(const bool loop=true) : TimerComponent(0.0, loop) {}

    CallbackTimer timer;
    Time startTime;
    bool loop;

    static void luaInit(LuaContext& lua);
};
}

#endif // TIMERCOMPONENT_HPP
