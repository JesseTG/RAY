#ifndef TIMERSYSTEM_HPP
#define TIMERSYSTEM_HPP

#include <anax/anax.hpp>

namespace ray {
using anax::System;
using anax::ComponentFilter;
class TimerSystem : public System<TimerSystem>
{
    public:
        TimerSystem();
        virtual ~TimerSystem();
        void update();
    private:
        static const ComponentFilter FILTER;
};
}

#endif // TIMERSYSTEM_HPP
