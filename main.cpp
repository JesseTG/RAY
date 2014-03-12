#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <anax/anax.hpp>
#include <LuaContext.hpp>

#include "config.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "entities.hpp"
#include "listeners.hpp"

int main()
{
    using std::vector;
    using std::chrono::milliseconds;
    using sf::Event;
    using sf::RenderWindow;
    using sf::VideoMode;
    using namespace ray;

    TractorBeamRepellingListener tb_listener;

    // Create the main window
    RenderWindow window(VideoMode(SCREEN_SIZE.x, SCREEN_SIZE.y), "SFML window");
    window.setFramerateLimit(60);
    anax::World world;
    b2World physics_world(b2Vec2(0, 0));
    LuaContext lua;

    entities::setWorld(world);
    entities::setRenderWindow(window);
    entities::setPhysicsWorld(physics_world);
    entities::setLuaState(lua);

    entities::initBodyDefs();
    entities::initBaseTypes();
    entities::initComponentLuaBindings();

    physics_world.SetContactListener(&tb_listener);

    Entity crosshair = entities::createMouseCircle(16); //entities::createEntity("MouseCircle", 16.0);
    Entity player = entities::createKeyboardCircle(crosshair, 32, 256, 256);
    Entity tractorbeam = entities::createTractorBeam(crosshair, player, 16, 0, 512, 1);

    FourWayControlSystem four_way_movement;
    RenderSystem rendering(window);
    MovementSystem movement;
    MouseFollowControlSystem mouse_following(window);
    FaceEntitySystem face_entity;
    EntityFollowSystem follow_entity;
    TractorBeamSystem tractor_system(tb_listener, tractorbeam);
    PhysicsSystem physics(physics_world);
    #ifdef DEBUG
    DebugSystem debug(window, physics_world, lua);
    #endif // DEBUG


    world.addSystem(four_way_movement);
    world.addSystem(mouse_following);
    //world.addSystem(movement);
    world.addSystem(face_entity);
    world.addSystem(follow_entity);
    world.addSystem(tractor_system);
    world.addSystem(physics);
    #ifdef DEBUG
    world.addSystem(debug);
    #endif // DEBUG
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
                default:
                    ; // nop
            }
        }
        // TODO: Improve timing
        // Provide a scaling factor and pass it into updates
        // Some things might move incorrectly if they depend on the framerate
        if (focused) {
            // If the player has the game window open...
            mouse_following.update();
            four_way_movement.update();
            //movement.update();
            face_entity.update();
            follow_entity.update();
            tractor_system.update();
            physics.update();
            #ifdef DEBUG
            debug.update(events);
            #endif // DEBUG
        }

        rendering.update();
        world.refresh();

        events.clear();
    }
}
