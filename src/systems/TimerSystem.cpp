#include "TimerSystem.hpp"

#include "components.hpp"
#include <iostream>
#include <Thor/Time.hpp>

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
        tc.timer.update();
        if (tc.loop && !tc.timer.isRunning()) {
            tc.timer.restart(tc.startTime);
            std::cout << "tick" << std::endl;
        }
    }
}

}
