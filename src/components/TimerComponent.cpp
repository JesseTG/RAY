#include "TimerComponent.hpp"

#include <boost/variant/get.hpp>
#include "util.hpp"

namespace ray {
using boost::get;

void TimerComponent::luaInit(LuaContext& lua) {
    lua.registerMember("timer", &TimerComponent::timer);
    lua.registerMember("loop", &TimerComponent::loop);
    lua.registerMember<TimerComponent, float>("startTime",
    [](const TimerComponent& timer) {
        return timer.startTime.asSeconds();
    },
    [](TimerComponent& timer, const float seconds) {
        timer.startTime = sf::seconds(seconds);
    });
    lua.writeFunction("TimerComponent", "new",
    [](const optional<float> arg1, const optional<bool> arg2) {
        if (arg1) {
            // If the user passed in one argument...
            return new TimerComponent(get<float>(arg1), arg2 ? *arg2 : true);
        }
        else {
            return new TimerComponent;
        }

    });
}

}
