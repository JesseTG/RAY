#include "TitleScreenState.hpp"

namespace ray {
TitleScreenState::TitleScreenState(GameManager& game) :
    _game(&game),
    _start_menu(sfg::Window::Create()),
    _start_button(sfg::Button::Create("Start Game"))
{
    this->_start_menu->SetTitle("Start Menu");
    this->_start_menu->Add(this->_start_button);
    this->_game->getDesktop()->Add(this->_start_menu);
    auto startButtonClicked = [this]() {
        LevelInfo level;
        level.name = "collide";
        this->_game->getStateMachine()->transition("swap", level);
    };
    this->_start_button->GetSignal( sfg::Button::OnLeftClick ).Connect(startButtonClicked);
}

TitleScreenState::~TitleScreenState()
{
    //dtor
}

void TitleScreenState::onEnter(World& w, GameStateArguments& arg) {
    this->_start_menu->Show(true);
    this->_game->getRenderWindow()->resetGLStates();
}

void TitleScreenState::onExit(World& w, GameStateArguments& arg) {
    this->_start_menu->Show(false);
}

void TitleScreenState::update(const vector<Event>& events) {
    RenderWindow& window = *this->_game->getRenderWindow();
    this->_game->getDesktop()->Update( 1.0f );
    window.clear(sf::Color::Magenta);
    for (const Event& e : events) {
        this->_game->getDesktop()->HandleEvent(e);
    }
    this->_game->getSfgui()->Display(window);
    window.display();
}
}
