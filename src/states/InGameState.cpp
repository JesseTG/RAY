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
    damage_listener(),
    contact_listener(),
    four_way_movement(),
    rendering(game),
    ai(*game.getLuaContext()),
    movement(),
    mouse_following(*game.getRenderWindow()),
    face_entity(),
    follow_entity(),
    tractor_system(tb_listener),
    physics(game.getPhysicsWorld()),
    _lives(0)
#ifdef DEBUG
    , debug(game)
#endif // DEBUG
{
    this->_load_level = this->_game->getLuaContext()->readVariable<function<void(string)>>("loadLevel");
    this->_gui->SetTitle("Health");
    this->_gui->Show(false);
    this->_health_bar->SetRequisition(sf::Vector2f(200.f, 20.f));
    this->_health_bar->SetFraction(0.5f);
    this->_health_box->Pack(this->_health_bar);

    this->_gui->Add(this->_health_box);
    game.getDesktop()->Add(this->_gui);
}

InGameState::~InGameState() {
}

void InGameState::onEnter(World& w, GameStateArguments& arg) {
    this->_game->resetWorld();
    this->_game->resetPhysicsWorld();
    this->contact_listener.addListener(&tb_listener);
    this->contact_listener.addListener(&damage_listener);
    this->_game->getPhysicsWorld()->SetContactListener(&contact_listener);

    this->_gui->Show(true);
    Entity crosshair = entities::createEntity("MouseCircle", 8.0);
    Entity player = entities::createEntity("KeyboardCircle", crosshair, 16, 4, 4);
    Entity tractorbeam = entities::createEntity("TractorBeam", crosshair, player, .25, 0, 10, .5);
    this->_game->setPlayer(player);

    LevelInfo* level = dynamic_cast<LevelInfo*>(&arg);

    this->_lives = level->lives;
    this->_load_level(level->name);

    w.addSystem(timer);
    w.addSystem(four_way_movement);
    w.addSystem(mouse_following);
    w.addSystem(ai);
    w.addSystem(face_entity);
    w.addSystem(tractor_system);
    w.addSystem(physics);
    w.addSystem(death);
#ifdef DEBUG
    w.addSystem(debug);
#endif // DEBUG
    w.addSystem(rendering);
    w.addSystem(removal);
    w.refresh();
}

void InGameState::onExit(World& w, GameStateArguments& arg) {
    this->contact_listener.clearListeners();
    this->_game->resetPhysicsWorld();
    this->_game->resetWorld();
    this->_gui->Show(false);
}

void InGameState::update(const vector<Event>& events) {
    timer.update();
    mouse_following.update();
    ai.update();
    four_way_movement.update();
    face_entity.update();
    tractor_system.update();
    physics.update();
    death.update();

#ifdef DEBUG
    debug.update(events);
#endif // DEBUG

    Entity player = this->_game->getPlayer();
    if (this->_game->getPlayer().isActivated()) {
        this->_health_bar->SetFraction(this->_game->getPlayer().getComponent<HealthComponent>().fraction());
    }
    else {
        this->_health_bar->SetFraction(0.0);
    }
    rendering.update();
    removal.update();
    this->_game->getWorld()->refresh();
}

}
