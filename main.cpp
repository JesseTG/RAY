#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>
#include <anax/anax.hpp>

#include "config.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "entities.hpp"

int main()
{
    using std::vector;
    using std::this_thread::sleep_for;
    using std::chrono::milliseconds;
    using sf::Event;
    using sf::RenderWindow;
    using sf::VideoMode;
    using namespace ray;

    // Create the main window
    RenderWindow window(VideoMode(SCREEN_SIZE.x, SCREEN_SIZE.y), "SFML window");
    window.setFramerateLimit(60);
    anax::World world;

    entities::setWorld(world);
    entities::setRenderWindow(window);

    Entity kbc = entities::createKeyboardCircle(32, 256, 256);
    Entity mc  = entities::createMouseCircle(16);

    FourWayControlSystem fwcs;
    RenderSystem rs(window);
    MovementSystem ms;
    MouseFollowControlSystem mfcs(window);
    PlayerGunSystem pgs(mc, window);

    world.addSystem(fwcs);
    world.addSystem(pgs);
    world.addSystem(mfcs);
    world.addSystem(ms);
    world.addSystem(rs);

    vector<Event> events;
    bool focused = true;
    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event))
        {
            events.push_back(event);

            // Close window : exit
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case Event::LostFocus:
                    focused = false;
                    break;
                case Event::GainedFocus:
                    focused = true;
                    break;
            }
        }

        if (focused) {
            pgs.update(events);
            mfcs.update();
            fwcs.update();
            ms.update();
        }

        rs.update();
        world.refresh();

        events.clear();
    }
}
