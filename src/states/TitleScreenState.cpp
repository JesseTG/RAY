#include "TitleScreenState.hpp"

namespace ray {
TitleScreenState::TitleScreenState(GameManager& game) :
    _game(&game),
    _start_menu(sfg::Window::Create()),
    _start_button(sfg::Button::Create("Start Game"))
{
    _splashScreenImg.loadFromFile("splashscreen.png");
    _splash_widget = sfg::Image::Create(_splashScreenImg);

    showMainMenu = false;
    this->_start_menu->SetTitle("Start Menu");
    this->_start_menu->Add(this->_start_button);
    this->_game->getDesktop()->Add(this->_start_menu);
    this->_game->getDesktop()->Add(this->_splash_widget);
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
    this->_start_menu->Show(false);
    this->_splash_widget->Show(true);
    this->_game->getRenderWindow()->resetGLStates();
}

void TitleScreenState::onExit(World& w, GameStateArguments& arg) {
    this->_start_menu->Show(false);
}

void TitleScreenState::update(const vector<Event>& events) {
    for (auto& e : events) {
        if (!showMainMenu && (e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Space)) {
            showMainMenu = true;
            this->_start_menu->Show(true);
            this->_splash_widget->Show(false);
        } else if (showMainMenu) {
            this->_game->getDesktop()->HandleEvent(e);
        }
    }
    RenderWindow& window = *this->_game->getRenderWindow();
    this->_game->getDesktop()->Update(1.0f);
    if (showMainMenu) {
        window.clear(sf::Color::Magenta);
    } else {
        window.clear(sf::Color::Blue);
    }
    this->_game->getSfgui()->Display(window);
    window.display();
}
}
