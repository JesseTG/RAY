#ifndef WANDERINGSYSTEM_HPP
#define WANDERINGSYSTEM_HPP

#include <anax/anax.hpp>
#include <Thor/Time/Timer.hpp>
#include <Thor/Math/Random.hpp>

namespace ray{
class WanderingSystem:public anax::System<WanderingSystem>
{
    public:
        WanderingSystem();
        void update();
    protected:
        thor::Timer time;
        float randX;
        float randY;
        float x;
        float y;
        float theta;
    private:
        static const anax::ComponentFilter FILTER;
};
}

#endif // WANDERINGSYSTEM_HPP
