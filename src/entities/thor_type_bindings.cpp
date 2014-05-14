#include "entities.hpp"

#include <cassert>
#include <functional>
#include <vector>
#include <LuaContext.hpp>
#include <Thor/Time.hpp>
#include <Thor/Input.hpp>
#include <SFML/System.hpp>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

namespace ray {
namespace entities {
using std::function;
using std::vector;
using boost::get;
using sf::Time;
using thor::Timer;
using thor::CallbackTimer;
using thor::Connection;
using thor::StopWatch;

template<class ThorTimerT>
void initCommonThorTimerBindings(const string& name, LuaContext& lua) {
    lua.registerFunction("start", &ThorTimerT::start);
    lua.registerFunction("stop", &ThorTimerT::stop);
    lua.registerFunction<ThorTimerT, void(float)>("reset",
    [](ThorTimerT& timer, const float seconds) {
        timer.reset(sf::seconds(seconds));
    });
    lua.registerFunction<ThorTimerT, void(float)>("restart",
    [](ThorTimerT& timer, const float seconds) {
        timer.restart(sf::seconds(seconds));
    });
    lua.registerFunction("isRunning", &ThorTimerT::isRunning);
    lua.registerFunction("isExpired", &ThorTimerT::isExpired);
}

void initThorTypeBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();

    lua.writeVariable("Thor", LuaEmptyArray);
    {
        lua.writeVariable("Thor", "Timer", LuaEmptyArray);
        {
            lua.writeFunction("Thor", "Timer", "new", getDefaultConstructorLambda<Timer>());
            initCommonThorTimerBindings<Timer>("Timer", lua);
        }

        lua.writeVariable("Thor", "CallbackTimer", LuaEmptyArray);
        {
            lua.writeFunction("Thor", "CallbackTimer", "new", getDefaultConstructorLambda<CallbackTimer>());
            typedef function<void(void)> nullary;
            lua.registerFunction<CallbackTimer, void(nullary)>("connect",
            [](CallbackTimer& timer, const nullary& f) {
                static vector<Connection> connections;
                Connection c = timer.connect0(f);
                connections.push_back(c);
                assert(connections.back().isConnected());
            });
            lua.registerFunction("clear", &CallbackTimer::clearConnections);
            lua.registerFunction("update", &CallbackTimer::update);
            initCommonThorTimerBindings<CallbackTimer>("CallbackTimer", lua);
            lua.registerFunction("start", &CallbackTimer::start);
        }

        lua.writeVariable("Thor", "StopWatch", LuaEmptyArray);
        {
            lua.writeFunction("Thor", "StopWatch", "new", getDefaultConstructorLambda<StopWatch>());
            lua.registerFunction("start", &StopWatch::start);
            lua.registerFunction("stop", &StopWatch::stop);
            lua.registerFunction<StopWatch, void(void)>("reset",
            [](StopWatch& timer) {
                timer.reset();
            });
            lua.registerFunction<StopWatch, void(void)>("restart",
            [](StopWatch& timer) {
                timer.restart();
            });
        }
    }
}
}
}
