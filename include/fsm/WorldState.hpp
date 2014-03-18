#ifndef WORLDSTATE_HPP
#define WORLDSTATE_HPP

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
using anax::detail::TypeId;

class WorldState
{
    public:
        WorldState() :
            _update([]() {}),
        _on_enter([]() {}),
        _on_exit([]() {})
        {}
        ~WorldState() {}

        template<class TSystem>
        void addSystem(TSystem& system, const int priority) {
#ifdef DEBUG
            if (this->_systems_by_priority.count(priority)) {
                // If we already have a System with the given priority...
                std::ostringstream err;
                err << "Duplicate priority " << priority << "for WorldState "
                    << this;
                throw logic_error(err.str());
            }
#endif // DEBUG
            this->_systems[TSystem::GetTypeId()] = &system;
            this->_systems_by_priority[priority] = &system;
        }

        template<class TSystem>
        void removeSystem() {
            this->_systems.erase(TSystem::GetTypeId());
        }

        void setUpdateCallback(const function<void(void)>& f) {
            this->_update = f;
        }

        void setOnEnterCallback(const function<void(void)>& f) {
            this->_on_enter = f;
        }

        void setOnExitCallback(const function<void(void)>& f) {
            this->_on_exit = f;
        }
    private:
        template<class, class> friend class WorldStateMachine;
        unordered_map<TypeId, BaseSystem*> _systems;
        map<int, BaseSystem*> _systems_by_priority;
        function<void(void)> _update;
        function<void(void)> _on_enter;
        function<void(void)> _on_exit;


};
}

#endif // WORLDSTATE_HPP
