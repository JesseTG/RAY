#ifndef FOURWAYMOVEMENTSYSTEM_HPP
#define FOURWAYMOVEMENTSYSTEM_HPP

#include <anax/anax.hpp>
#include <SFML/Graphics.hpp>

namespace ray
{

using anax::System;
using anax::Entity;
using anax::ComponentFilter;

using sf::Keyboard;
using sf::Event;
typedef Keyboard::Key Key;

/**
 * Handles the movement of able @c Entities in the eight cardinal directions.
 *
 * All @c Entities in this FourWayControlSystem must have a FourWayControlComponent,
 * a PositionComponent, and a VelocityComponent, and cannot have a
 * MouseFollowControlComponent.
 */
class FourWayControlSystem : public System<FourWayControlSystem>
{
    public:
        /**
         * Constructor.
         */
        FourWayControlSystem();

        /**
         * Destructor.
         */
        virtual ~FourWayControlSystem();

        /**
         * Moves all stored @c Entities horizontally or vertically depending
         * on which keys are pressed.
         */
        void update();
    private:
        static const ComponentFilter FILTER;
};
}

#endif // FOURWAYMOVEMENTSYSTEM_HPP
