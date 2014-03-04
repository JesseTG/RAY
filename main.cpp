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

    Entity player = entities::createKeyboardCircle(32, 256, 256);
    Entity crosshair = entities::createMouseCircle(16);
    Entity tractorbeam = entities::createTractorBeam(crosshair, player, 16, 0, 128, 1);

    FourWayControlSystem four_way_movement;
    RenderSystem rendering(window);
    MovementSystem movement;
    MouseFollowControlSystem mouse_following(window);
    PlayerGunSystem player_gun(player, window);
    FaceEntitySystem face_entity;
    EntityFollowSystem follow_entity;
    TractorBeamSystem tractor_system(tractorbeam, window);

    world.addSystem(four_way_movement);
    world.addSystem(player_gun);
    world.addSystem(mouse_following);
    world.addSystem(movement);
    world.addSystem(face_entity);
    world.addSystem(follow_entity);
    world.addSystem(tractor_system);
    world.addSystem(rendering);

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
        // TODO: Improve timing
        // Provide a scaling factor and pass it into updates
        // Some things might move incorrectly if they depend on the framerate
        if (focused) {
            // If the player has the game window open...
            player_gun.update(events);
            mouse_following.update();
            four_way_movement.update();
            movement.update();
            face_entity.update();
            follow_entity.update();
            tractor_system.update();
        }

        rendering.update();
        world.refresh();

        events.clear();
    }
}
