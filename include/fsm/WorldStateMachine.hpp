#ifndef WORLDSTATEMACHINE_HPP
#define WORLDSTATEMACHINE_HPP

#include <exception>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <utility>

#ifdef DEBUG
#include <unordered_set>
#endif // DEBUG

#include <anax/anax.hpp>

#include "WorldState.hpp"

namespace util {
using std::logic_error;
using std::make_pair;
using std::map;
using std::get;
using std::ostringstream;
using std::pair;
using std::string;
using std::stringstream;
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
 */
template<class TStateKey, class TAction>
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
            const map<TStateKey, WorldState>& states,
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
                        throw logic_error(err.str());
                    }


                    for (const auto& j : w_keys) {
                        // Now for every state key we got as an argument...
                        if (!states.count(j)) {
                            // If we don't actually have a state mapped to it...
                            std::ostringstream err;
                            err << "State key " << j << " in WorldStateMachine "
                                << this << " has no mapped state";
                            throw logic_error(err.str());
                        }
                    }
                #endif //DEBUG
                _current_state = this->_states.find(start);
        }

        /**
         *
         * @throw @c std::logic_error for an undefined transition
         */
        const WorldState& transition(const TAction& symbol) {
            auto inputpair = make_pair(symbol, this->_current_state->first);
            if (this->_transitions.count(inputpair)) {
                // If we have a transition defined given our current state and
                // and input symbol...
                if (this->_states.count(this->_transitions.find(inputpair)->second)) {
                    // If we have a state defined for the given state key...
                    this->_current_state->second._on_exit();
                    this->_current_state = this->_states.find(inputpair.second);
                    this->_current_state->second._on_enter();
                    return this->_states[this->_current_state->first];
                }
                else {
                    ostringstream err;
                    err << "No state with key " << inputpair.second
                        << " found (fsm=" << this << ")";
                    throw logic_error(err.str());
                }
            }
            else {
                ostringstream err;
                err << "No transition found: (fsm=" << this << ", symbol="
                    << inputpair.first << ", state=" << inputpair.second
                    << ")";
                throw logic_error(err.str());
            }
        }

        const WorldState& currentState() const {
            return this->_states[this->_current_state->second];
        }

        void update() {
            this->_current_state->second._update();
        }


        void addTransition(
            const TStateKey& currentkey,
            const TAction& inpuTAction,
            const TStateKey& nextstate) {
            this->_transitions[make_pair(inpuTAction, currentkey)] = nextstate;
        }
    private:
        World* _world;
        unordered_map<TStateKey, WorldState> _states;
        map<pair<TAction, TStateKey>, TStateKey> _transitions;
        // TODO: Make this an unordered_map
        typename decltype(_states)::iterator _current_state;
};
}

#endif // WORLDSTATEMACHINE_HPP
