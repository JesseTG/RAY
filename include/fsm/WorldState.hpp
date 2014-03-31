#ifndef WORLDSTATE_HPP
#define WORLDSTATE_HPP

#include <anax/anax.hpp>

#include "fsm.hpp"

namespace util {

using anax::World;

/**
 * Represents a state (a screen, basically) the game can be  in (pause menu,
 * in-game, inventory, etc.).
 *
 * @tparam UpdateArguments The arguments that the update function will take.
 */
template<typename...UpdateArguments>
class WorldState
{
    public:
        virtual ~WorldState() {}

    protected:
        template<class, class, typename...> friend class WorldStateMachine;

        /**
         * Called when another @c WorldState transitions to this one. By default,
         * does nothing; feel free to override this.
         *
         * @param w The @c anax::World governed by @c *this.
         */
        virtual void onEnter(World& w) {}

        /**
         * Called when @c *this transitions to another WorldState. By default,
         * does nothing; feel free to override this.
         *
         * @note This will be called before the next state's @c onEnter method
         * will.
         *
         * @param w The @c anax::World governed by @c *this.
         */
        virtual void onExit(World& w) {}

        /**
         * Updates any pertinent world state (@c Systems, @c Entities, etc.).
         *
         * @param args Arguments passed in for the @c Systems to act upon.
         */
        virtual void update(const UpdateArguments&...) = 0;
};
}

#endif // WORLDSTATE_HPP
