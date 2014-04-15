#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>
#include <string>
#include <utility>
#include <memory>

#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
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
#include "ai.hpp"

#include <SFGUI/SFGUI.hpp>

int main()
{
    using std::vector;
    using std::string;
    using std::make_pair;
    using std::chrono::milliseconds;
    using sf::Event;
    using sf::ContextSettings;
    using sf::RenderWindow;
    using sf::VideoMode;
    using sf::View;
    using namespace util;
    using namespace ray;

    sfg::SFGUI sfgui;

    TractorBeamRepellingListener tb_listener;
    GameManager gm;
    RenderWindow& window = *gm.getRenderWindow();
    // Create the main window
    entities::setLuaState(gm.getLuaContext());

    entities::initBaseTypes(gm);
    entities::initManagerTypeBindings(gm);
    entities::initComponentLuaBindings(gm);
    ai::initAIBindings(gm);

    gm.getScriptManager()->loadConfigFile("data/script/scripts.json");
    gm.getShapeManager()->loadConfigFile("data/shape/shapes.json");
    gm.getPhysicsWorld()->SetContactListener(&tb_listener);

    FourWayControlSystem four_way_movement;
    RenderSystem rendering(*gm.getRenderWindow().get());
    AISystem ai(*gm.getLuaContext());
    MovementSystem movement;
    MouseFollowControlSystem mouse_following(*gm.getRenderWindow().get());
    FaceEntitySystem face_entity;
    EntityFollowSystem follow_entity;
    TractorBeamSystem tractor_system(tb_listener);
    PhysicsSystem physics(gm.getPhysicsWorld().get());
#ifdef DEBUG
    DebugSystem debug(gm);
#endif // DEBUG

    auto gameEnter = [&](World& w) {
        gm.resetPhysicsWorld();
        gm.getPhysicsWorld()->SetContactListener(&tb_listener);
        physics.setWorld(gm.getPhysicsWorld().get());

        Entity crosshair = entities::createEntity("MouseCircle", 8.0);
        Entity player = entities::createEntity("KeyboardCircle", crosshair, 16, 256, 256);
        Entity tractorbeam = entities::createEntity("TractorBeam", crosshair, player, 16, 0, 512, 1);
        GameShape s = gm.getShapeManager()->getShape("collide");
        for (int i = 0; i < s.graphics_shapes.size(); ++i) {
            Vector2f pos = dynamic_pointer_cast<Transformable>(s.graphics_shapes[i])->getPosition();
            Entity e = w.createEntity();
            e.addComponent<RenderableComponent>(new RenderableComponent(s.graphics_shapes[i]));
            e.addComponent<PositionComponent>(pos);

            if (s.physics_shapes[i]) {
                b2BodyDef bdef;
                bdef.awake = true;
                bdef.active = true;
                bdef.position = b2Vec2(pos.x, pos.y);
                bdef.type = b2BodyType::b2_staticBody;
                bdef.fixedRotation = true;
                b2FixtureDef fdef;
                fdef.shape = s.physics_shapes[i].get();
                b2Body* body = gm.getPhysicsWorld()->CreateBody(&bdef);
                b2Fixture* fixture = body->CreateFixture(&fdef);
                PhysicsBodyComponent* pbc = new PhysicsBodyComponent(body, e);
                PhysicsFixtureComponent* pfc = new PhysicsFixtureComponent(fixture, e);
                e.addComponent<PhysicsBodyComponent>(pbc);
                e.addComponent<PhysicsFixtureComponent>(pfc);
            }

            e.activate();
        }


        w.addSystem(four_way_movement);
        w.addSystem(mouse_following);
        w.addSystem(ai);
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
        ai.update();
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
        // Can't call w.clear(), it causes a segfault if you go back to this
        // state later; a bug in Anax?
        gm.resetPhysicsWorld();
    };

    sfg::Desktop desktop;

    auto startEnter = [](World& w) {};
    auto startUpdate = [&](const vector<Event>& events) {
        desktop.Update( 1.0f );
        window.clear(sf::Color::Magenta);
        for (auto& e : events) {
            desktop.HandleEvent(e);
        }
        sfgui.Display(*gm.getRenderWindow());
        window.display();
    };
    auto startExit = [](World& w) {};

    WorldStateMachine<string, string, vector<Event>> wsm(*gm.getWorld(), "start",
    {
        {"start",  make_shared<CompositionWorldState<vector<Event>>>(startUpdate, startEnter, startExit)},
        {"game", make_shared<CompositionWorldState<vector<Event>>>(gameUpdate, gameEnter, gameExit)},
    },
    {
        {make_pair("swap", "start"), "game"},
        {make_pair("swap", "game"), "start"},
    });

    // start menu gui
    auto startButton = sfg::Button::Create("Start Game");
    auto startButtonClicked = [&wsm]() {
        wsm.transition("swap");
    };
    startButton->GetSignal( sfg::Button::OnLeftClick ).Connect(startButtonClicked);
    auto startMenu = sfg::Window::Create();
    startMenu->SetTitle( "Start Menu" );
    startMenu->Add( startButton );
    desktop.Add( startMenu );
    window.resetGLStates();

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
