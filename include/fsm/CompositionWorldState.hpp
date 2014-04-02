#ifndef COMPOSITIONWORLDSTATE_HPP
#define COMPOSITIONWORLDSTATE_HPP

#include "WorldState.hpp"

#include <exception>
#include <functional>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#ifdef DEBUG
#include <sstream>
#endif // DEBUG

#include <anax/anax.hpp>

#include "fsm.hpp"

namespace util {
using std::logic_error;
using std::map;
using std::unordered_map;
using std::function;
using std::vector;
using anax::BaseSystem;
using anax::World;
using anax::detail::TypeId;

template<typename...UpdateArguments>
class CompositionWorldState : public WorldState<UpdateArguments...>
{
    public:
        typedef function<void(const UpdateArguments& ...)> UpdateFunction;
        CompositionWorldState(
        const UpdateFunction& update=[](const UpdateArguments& ...) {},
        const function<void(World&)>& onenter=[](World&) {},
        const function<void(World&)>& onexit=[](World&) {}
        ) :
            _update(update),
            _on_enter(onenter),
            _on_exit(onexit)
        {}

        ~CompositionWorldState() {}

        void setUpdateCallback(const UpdateFunction& f) {
            this->_update = f;
        }

        void setOnEnterCallback(const function<void(World&)>& f) {
            this->_on_enter = f;
        }

        void setOnExitCallback(const function<void(World&)>& f) {
            this->_on_exit = f;
        }
    protected:
        void onEnter(World& world) override {
            this->_on_enter(world);
        }

        void onExit(World& world) override {
            this->_on_exit(world);
        }

        void update(const UpdateArguments& ...args) override {
            this->_update(args...);
        }
    private:
        template<class, class, typename...> friend class WorldStateMachine;
        UpdateFunction _update;
        function<void(World&)> _on_enter;
        function<void(World&)> _on_exit;
};
}

#endif // COMPOSITIONWORLDSTATE_HPP
