#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>
#include <string>
#include <utility>
#include <memory>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <anax/anax.hpp>
#include <LuaContext.hpp>

#include "config.hpp"
#include "components.hpp"
#include "fsm.hpp"
#include "systems.hpp"
#include "entities.hpp"
#include "managers.hpp"
#include "listeners.hpp"

int main()
{
    using std::vector;
    using std::string;
    using std::make_pair;
    using std::chrono::milliseconds;
    using sf::Event;
    using sf::RenderWindow;
    using sf::VideoMode;
    using namespace util;
    using namespace ray;

    TractorBeamRepellingListener tb_listener;
    GameManager gm;

    // Create the main window
    RenderWindow window(VideoMode(SCREEN_SIZE.x, SCREEN_SIZE.y), "SFML window");
    window.setFramerateLimit(FPS);

    entities::setWorld(gm.getWorld());
    entities::setRenderWindow(window);
    entities::setPhysicsWorld(gm.getPhysicsWorld());
    entities::setLuaState(gm.getLuaContext());

    entities::initBaseTypes();
    entities::initComponentLuaBindings();

    std::ifstream in("data/script/entities.lua");
    gm.getLuaContext()->executeCode(in);

    gm.getPhysicsWorld()->SetContactListener(&tb_listener);

    FourWayControlSystem four_way_movement;
    RenderSystem rendering(window);
    MovementSystem movement;
    MouseFollowControlSystem mouse_following(window);
    FaceEntitySystem face_entity;
    EntityFollowSystem follow_entity;
    TractorBeamSystem tractor_system(tb_listener);
    PhysicsSystem physics(gm.getPhysicsWorld().get());
#ifdef DEBUG
    DebugSystem debug(window, gm);
#endif // DEBUG

    auto gameEnter = [&](World& w) {
        gm.resetPhysicsWorld();
        entities::setPhysicsWorld(gm.getPhysicsWorld());
        gm.getPhysicsWorld()->SetContactListener(&tb_listener);
        physics.setWorld(gm.getPhysicsWorld().get());

        Entity crosshair = entities::createEntity("MouseCircle", 16.0);
        Entity player = entities::createEntity("KeyboardCircle", crosshair, 32, 256, 256);
        Entity tractorbeam = entities::createEntity("TractorBeam", crosshair, player, 16, 0, 512, 1);

        w.addSystem(four_way_movement);
        w.addSystem(mouse_following);
        w.addSystem(face_entity);
        w.addSystem(follow_entity);
        w.addSystem(tractor_system);
        w.addSystem(physics);
#ifdef DEBUG
        w.addSystem(debug);
#endif // DEBUG
        w.addSystem(rendering);
        w.refresh();
    };

    auto gameUpdate = [&](const vector<Event>& e) {
        mouse_following.update();
        four_way_movement.update();
        face_entity.update();
        follow_entity.update();
        tractor_system.update();
        physics.update();
#ifdef DEBUG
        debug.update(e);
#endif // DEBUG
        rendering.update();
        gm.getWorld()->refresh();
    };

    auto gameExit = [&](World& w) {
        auto ent = w.getEntities();
        w.killEntities(ent);
        w.removeAllSystems();
        gm.resetPhysicsWorld();
    };

    auto startEnter = [](World& w) {};
    auto startUpdate = [&window](const vector<Event>&) {
        window.clear(sf::Color::Magenta);
        window.display();
    };
    auto startExit = [](World& w) {};

    WorldStateMachine<string, string, vector<Event>> wsm(*gm.getWorld(), "start",
    {
        {"start", make_shared<CompositionWorldState<vector<Event>>>(gameUpdate, gameEnter, gameExit)},
        {"game",  make_shared<CompositionWorldState<vector<Event>>>(startUpdate, startEnter, startExit)}
    },
    {
        {make_pair("swap", "start"), "game"},
        {make_pair("swap", "game"), "start"},
    });

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
                case Event::KeyPressed:
                    if (event.key.code == Keyboard::G) {
                        wsm.transition("swap");
                    }
                    break;
                default:
                    ; // nop
            }
        }

        wsm.update(events);

        events.clear();
    }
}

