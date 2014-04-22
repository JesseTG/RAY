#ifndef WORLDSTATEMACHINE_HPP
#define WORLDSTATEMACHINE_HPP

#include <exception>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <anax/anax.hpp>

#include "fsm.hpp"
#include "LevelInfo.hpp"

namespace util {
using std::make_pair;
using std::make_shared;
using std::map;
using std::get;
using std::pair;
using std::string;
using std::shared_ptr;
using std::unordered_map;
using anax::World;

/**
 * A state machine for transitioning between game states that is consistent with
 * the definition of a <a href="http://en.wikipedia.org/wiki/Finite_automaton">
 * finite automaton</a>.
 *
 * A WorldState will be the state of the automaton, and a symbol in the
 * automaton will be some user-defined type (typically either a @c std::string
 * or an @c enum). Whatever the symbol will be, it'll usually be defined as a verb
 * (e.g. @c "exit" or @c "pause", or even better, a hypothetical enum @c UserAction::Exit
 * or @c UserAction::Pause).
 *
 * Once initialized, you can't add or remove any states. This is by design.
 *
 * @tparam TStateKey The type used to refer to a WorldState. Preferably an @c enum.
 * @tparam TAction The type used to refer to the transition action (in pure
 * FSM terms, a symbol). Ideally an @c enum.
 * @tparam UpdateArgument Some argument passed into a WorldState
 */
template<class TStateKey, class TAction, typename...UpdateArguments>
class WorldStateMachine
{
    public:
        /**
         * Constructor.
         *
         * Initializes a WorldStateMachine.
         *
         * In Debug builds, ensures that each state in the transition table
         * actually exists.
         *
         * @param world The @c anax::World to be controlled by @c *this
         * @param start The key for the initial state
         * @param states A mapping of keys to actual states. Preferably passed
         * in with argument list syntax.
         * @param transitions A mapping of a pair containing the input action and
         * the current state key to the resulting state key.
         */
        WorldStateMachine(
            World& world,
            const TStateKey& start,
            const map<TStateKey, shared_ptr<WorldState<UpdateArguments...>>>& states,
            const map<pair<TAction, TStateKey>, TStateKey>& transitions
        ) :
            _world(&world),
            _states(states.begin(), states.end()),
            _transitions(transitions.begin(), transitions.end()) {
                #ifdef DEBUG
                    std::unordered_set<TStateKey> w_keys;
                    w_keys.reserve(states.size());
                    for (const auto& i : transitions) {
                        // For each transition in the transition table...
                        w_keys.insert(i.second);
                        w_keys.insert(i.first.second);
                        // Add every state key we're transitioning to or from
                    }

                    if (states.size() != w_keys.size()) {
                        // If we don't have one state for every key...
                        std::ostringstream err;
                        err << "Extra states or state keys found";
                        throw std::logic_error(err.str());
                    }

                    for (const auto& j : w_keys) {
                        // Now for every state key we got as an argument...
                        if (!states.count(j)) {
                            // If we don't actually have a state mapped to it...
                            std::ostringstream err;
                            err << "State key " << j << " in WorldStateMachine "
                                << this << " has no mapped state";
                            throw std::logic_error(err.str());
                        }
                    }
                #endif //DEBUG
                ray::LevelInfo a;
                _current_state = this->_states.find(start);
                _current_state->second->onEnter(*this->_world, a);
        }

        /**
         * Given the current @c WorldState and the given input, decides which
         * @c WorldState to go to next.
         *
         * @param symbol The action that will govern what state to go to next
         * (e.g. @c EXIT, @c PAUSE, etc.).
         * @throw @c std::logic_error for an undefined transition or a state
         * key without a corresponding state.
         */
        const WorldState<UpdateArguments...>& transition(const TAction& symbol, GameStateArguments& arg) {
            pair<TAction, TStateKey> inputpair = make_pair(symbol, this->_current_state->first);
            if (this->_transitions.count(inputpair)) {
                // If we have a transition defined given our current state and
                // and input symbol...
                const TStateKey& newkey = this->_transitions.find(inputpair)->second;
                if (this->_states.count(newkey)) {
                    // If we have a state defined for the given state key...
                    this->_current_state->second->onExit(*this->_world, arg);
                    this->_current_state = this->_states.find(newkey);
                    this->_current_state->second->onEnter(*this->_world, arg);
                    return *this->_states[this->_current_state->first];
                }
                else {
                    std::ostringstream err;
                    err << "No state with key " << inputpair.second
                        << " found (fsm=" << this << ")";
                    throw std::logic_error(err.str());
                }
            }
            else {
                std::ostringstream err;
                err << "No transition found: (fsm=" << this << ", symbol="
                    << inputpair.first << ", state=" << inputpair.second
                    << ")";
                throw std::logic_error(err.str());
            }
        }

        /**
         * Returns the current @c WorldState.
         */
        const WorldState<UpdateArguments...>& currentState() const {
            return this->_states[this->_current_state->second];
        }

        /**
         * Updates the underlying @c World and its @c Systems. Ideally called
         * once per frame.
         *
         * @param args The arguments as specified by the @c UpdateArguments
         * template parameter
         */
        void update(const UpdateArguments&...args) {
            this->_current_state->second->update(args...);
        }
    private:
        World* _world;
        unordered_map<TStateKey, shared_ptr<WorldState<UpdateArguments...>>> _states;
        map<pair<TAction, TStateKey>, TStateKey> _transitions;
        // ^ TODO: Make this an unordered_map
        typename decltype(_states)::iterator _current_state;
};
}

#endif // WORLDSTATEMACHINE_HPP
