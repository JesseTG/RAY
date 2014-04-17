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

    TractorBeamRepellingListener tb_listener;
    GameManager gm;
    RenderWindow& window = *gm.getRenderWindow();
    // Create the main window
    entities::setLuaState(gm.getLuaContext());

    entities::initBaseTypes(gm);
    entities::initManagerTypeBindings(gm);
    entities::initComponentLuaBindings(gm);
    ai::initAIBindings(gm);

    gm.getPhysicsWorld()->SetContactListener(&tb_listener);

    FourWayControlSystem four_way_movement;
    RenderSystem rendering(gm);
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

    auto startMenu = sfg::Window::Create();
    gm.getDesktop()->Add( startMenu );
    auto startButton = sfg::Button::Create("Start Game");
    startMenu->SetTitle( "Start Menu" );
    startMenu->Add( startButton );
    gm.getDesktop()->Add( startMenu );
    sfg::Window::Ptr progressBar = sfg::Window::Create();
    progressBar->SetTitle("Health");
    sfg::ProgressBar::Ptr health = sfg::ProgressBar::Create();
    health->SetRequisition( sf::Vector2f( 200.f, 20.f ) );
    sfg::Box::Ptr healthBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.0f);
    healthBox->Pack(health);
    progressBar->Add(healthBox);
    gm.getDesktop()->Add(progressBar);
    health->SetFraction(0.5f);
    progressBar->Show(false);

    auto gameEnter = [&](World& w) {
        gm.resetPhysicsWorld();
        gm.getPhysicsWorld()->SetContactListener(&tb_listener);
        physics.setWorld(gm.getPhysicsWorld().get());

        Entity crosshair = entities::createEntity("MouseCircle", 8.0);
        Entity player = entities::createEntity("KeyboardCircle", crosshair, 16, 256, 256);
        gm.setPlayer(player);
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
        startMenu->Show(false);
        progressBar->Show(true);
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
        progressBar->Show(false);
    };

    auto startEnter = [&gm, &startMenu](World& w) {
        startMenu->Show(true);
    };
    auto startUpdate = [&](const vector<Event>& events) {
        gm.getDesktop()->Update( 1.0f );
        window.clear(sf::Color::Magenta);
        for (auto& e : events) {
            gm.getDesktop()->HandleEvent(e);
        }
        gm.getSfgui()->Display( window );
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

    auto startButtonClicked = [&wsm]() { wsm.transition("swap"); };
    startButton->GetSignal( sfg::Button::OnLeftClick ).Connect(startButtonClicked);
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
