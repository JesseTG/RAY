#include "TimerSystem.hpp"

#include "components.hpp"
#include <iostream>

namespace ray {

const ComponentFilter TimerSystem::FILTER = ComponentFilter()
    .requires<TimerComponent>();

TimerSystem::TimerSystem() : Base(FILTER)
{
    //ctor
}

TimerSystem::~TimerSystem()
{
    //dtor
}

void TimerSystem::update() {
    for (Entity& e : this->getEntities()) {
        TimerComponent& tc = e.getComponent<TimerComponent>();
        if (tc.loop && tc.timer.isExpired()) {
            tc.timer.restart(tc.startTime);
        }
        tc.timer.update();
    }
}

}
