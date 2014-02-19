#ifndef ENTITYSTATEMACHINE_HPP
#define ENTITYSTATEMACHINE_HPP

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

#include <anax/anax.hpp>

#include "fsm.hpp"


namespace util {

using std::is_enum;
using std::logic_error;
using std::unordered_map;
using std::unique_ptr;

using anax::Entity;
using anax::detail::TypeId;

/**
 * We're using an @c enum here to mitigate the dangers of typos, i.e. we'd
 * prefer you use
 * @code esm.createState(EnemyStates::DYING) @endcode
 * over
 * @code esm.createState(DYING) @endcode
 * (where @c DYING is a @c #define or a constant integer) or
 * @code esm.createState("dying") @endcode
 *
 */
class EntityStateMachine
{
    public:

        EntityStateMachine(Entity& e);
        ~EntityStateMachine();

        /**
         * Returns the underlying @c Entity.
         */
        Entity entity() const;

        /**
         * Creates a new EntityState mapped to the given @c enum and returns a
         * reference to it.
         *
         * @tparam E The @c enum type that represents the set of all possible
         * states the underlying @c Entity should be in.
         * @param state The @c enum value that the newly-created EntityState
         * will be mapped to.
         */
        template<class E>
        EntityState& createState(const E state) {
            static_assert(is_enum<E>::value, "E should be an enum or enum class");

            return this->_create_state(static_cast<int>(state));
        }

        /**
         * Changes the current state
         */
        template<class E>
        EntityState& changeState(const E state) {
            static_assert(is_enum<E>::value, "E should be an enum or enum class");

            return this->_change_state(static_cast<int>(state));
        }

        /**
         * Removes the given EntityState and returns true if it was removed
         */
        template<class E>
        bool removeState(const E state) {
            return this->_states.erase(static_cast<int>(state)) > 0;
        }

        template<class E>
        bool has(const E state) {
            static_assert(is_enum<E>::value, "E should be an enum or enum class");

            return this->_has(static_cast<int>(state));
        }
    private:
        Entity _entity;
        unique_ptr<EntityState>* _current_state;
        unordered_map<int, unique_ptr<EntityState>> _states;

        EntityState& _create_state(const int state);
        EntityState& _change_state(const int state);
        bool _has(const int state) const;

};
}

#endif // ENTITYSTATEMACHINE_HPP
