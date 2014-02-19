#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include <anax/anax.hpp>

namespace ray {

using anax::ComponentFilter;
using anax::Entity;
using anax::System;

/**
 * Moves all controlled @c Entities just a little bit each frame.
 *
 * All @c Entities in this MovementSystem must have a PositionComponent and a
 * VelocityComponent.
 */
class MovementSystem : public System<MovementSystem>
{
    public:
        /**
         * Constructor.
         */
        MovementSystem();

        /**
         * Destructor.
         */
        ~MovementSystem();

        /**
         * When called, offsets each controlled @c Entitie's PositionComponent
         * @c Vector2 by whatever @c Vector2 its VelocityComponent contains.
         */
        void update();
    private:
        static const ComponentFilter FILTER;
};
}

#endif // MOVEMENTSYSTEM_HPP
