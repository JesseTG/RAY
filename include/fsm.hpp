#ifndef FSM_HPP
#define FSM_HPP

namespace util {
class EntityState;
class EntityStateMachine;
struct GameStateArguments;
template<typename...> class WorldState;
template<typename...> class CompositionWorldState;
template<class, class, typename...> class WorldStateMachine;
}

#include "EntityState.hpp"
#include "EntityStateMachine.hpp"
#include "GameStateArguments.hpp"
#include "WorldState.hpp"
#include "WorldStateMachine.hpp"
#include "CompositionWorldState.hpp"

#endif // FSM_HPP
