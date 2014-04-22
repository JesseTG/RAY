#include "InGameState.hpp"

#include <iostream>
#include <SFML/System.hpp>

#include "entities.hpp"

namespace ray {

using boost::any_cast;

InGameState::InGameState(GameManager& game) :
    _gui(sfg::Window::Create()),
    _health_bar(sfg::ProgressBar::Create()),
    _health_box(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.0f)),
    _game(&game),
    tb_listener(),
    four_way_movement(),
    rendering(game),
    ai(*game.getLuaContext()),
    movement(),
    mouse_following(*game.getRenderWindow()),
    face_entity(),
    follow_entity(),
    tractor_system(tb_listener),
    physics(game.getPhysicsWorld().get())
#ifdef DEBUG
    , debug(game)
#endif // DEBUG
{
    this->_gui->SetTitle("Health");
    this->_gui->Show(false);
    this->_health_bar->SetRequisition( sf::Vector2f( 200.f, 20.f ) );
    this->_health_bar->SetFraction(0.5f);
    this->_health_box->Pack(this->_health_bar);

    this->_gui->Add(this->_health_box);
    game.getDesktop()->Add(this->_gui);
}

InGameState::~InGameState() {
}

void InGameState::onEnter(World& w, GameStateArguments& arg) {
    this->_game->resetPhysicsWorld();
    this->_game->getPhysicsWorld()->SetContactListener(&tb_listener);
    this->physics.setWorld(this->_game->getPhysicsWorld().get());
    this->_gui->Show(true);
    Entity crosshair = entities::createEntity("MouseCircle", 8.0);
    Entity player = entities::createEntity("KeyboardCircle", crosshair, 16, 256, 256);
    this->_game->setPlayer(player);
    Entity tractorbeam = entities::createEntity("TractorBeam", crosshair, player, 16, 0, 512, 1);

    LevelInfo* level = dynamic_cast<LevelInfo*>(&arg);
    _game->getLuaContext()->readVariable<function<void(string)>>("loadLevel")(level->name);

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
}

void InGameState::onExit(World& w, GameStateArguments& arg) {
    auto ent = w.getEntities();
    w.killEntities(ent);
    w.removeAllSystems();
    // Can't call w.clear(), it causes a segfault if you go back to this
    // state later; a bug in Anax?
    this->_game->resetPhysicsWorld();
    this->_gui->Show(false);
}

void InGameState::update(const vector<Event>& events) {
    mouse_following.update();
    ai.update();
    four_way_movement.update();
    face_entity.update();
    follow_entity.update();
    tractor_system.update();
    physics.update();
#ifdef DEBUG
    debug.update(events);
#endif // DEBUG
    rendering.update();
    this->_game->getWorld()->refresh();
}

}
