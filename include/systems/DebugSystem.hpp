#ifndef DEBUGSYSTEM_HPP
#define DEBUGSYSTEM_HPP

#include <vector>

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <LuaContext.hpp>

namespace ray {

using std::vector;

using sf::RenderWindow;
using sf::Event;


/**
 * A @c System to aid in debugging. Use it to add @c Entities on command, to
 * change information, to print things out on-screen, etc. Does nothing when the
 * game is compiled in Release mode (in fact, it shouldn't even be added in
 * Debug mode).
 *
 * DebugSystem should have access to as much data about the game as possible.
 * At the moment, it maintains references to the game's @c sf::RenderWindow and
 * the @c b2World.
 *
 * @note You can already get the @c anax::World by calling @c this->getWorld().
 * That's a method for subclasses of @c anax::System.
 */
class DebugSystem : public anax::System<DebugSystem>
{
    public:
        /**
         * Constructor. Initializes this DebugSystem with a @c sf::RenderWindow,
         * a @c b2World, and a LuaContext.
         */
        DebugSystem(RenderWindow&, b2World&, LuaContext&);
        ~DebugSystem();

        void update(const vector<Event>& events);
    private:
        RenderWindow* _window;
        b2World* _physics_world;
        LuaContext* _lua;
};
}

#endif // DEBUGSYSTEM_HPP
