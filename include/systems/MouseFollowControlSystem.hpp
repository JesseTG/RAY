#ifndef MOUSEFOLLOWCONTROLSYSTEM_HPP
#define MOUSEFOLLOWCONTROLSYSTEM_HPP

#include <vector>

#include <anax/anax.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "components.hpp"

namespace ray
{

using std::vector;

using anax::System;
using anax::ComponentFilter;
using anax::Entity;

using sf::Event;
using sf::Mouse;
using sf::RenderWindow;
using sf::Vector2;
using sf::Vector2i;
using sf::Vector2f;

/**
 * Makes qualified @c Entities follow the mouse closely.
 *
 * All controlled @c Entities must have a MouseFollowControlComponent and a
 * PositionComponent, and cannot have a FourWayControlComponent.
 */
class MouseFollowControlSystem : public System<MouseFollowControlSystem>
{
    public:
        /**
         * Constructor.
         *
         * @param window The @c RenderWindow used to provide the offset
         * so that the mouse coordinates are specified in terms of the game
         * window, not the entire screen.
         */
        MouseFollowControlSystem(RenderWindow& window);

        /**
         * Destructor.
         */
        virtual ~MouseFollowControlSystem();

        /**
         * When called, moves all managed @c Entities to wherever the mouse
         * cursor is.
         */
        void update();
    private:
        RenderWindow* _window;
        const static ComponentFilter FILTER;
};
}

#endif // MOUSEFOLLOWCONTROLSYSTEM_HPP
