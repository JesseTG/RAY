#ifndef TITLESCREENSTATE_HPP
#define TITLESCREENSTATE_HPP

#include <vector>

#include <SFGUI/SFGUI.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <anax/anax.hpp>

#include "config.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "listeners.hpp"
#include "fsm.hpp"

namespace ray {
using boost::any;
using std::vector;
using sf::Color;
using sf::Event;
using sf::RenderWindow;
using anax::World;
using util::GameStateArguments;
class TitleScreenState : public util::WorldState<vector<Event>>
{
    public:
        TitleScreenState(GameManager& game);
        virtual ~TitleScreenState();

        void onEnter(World& w, GameStateArguments&) final override;

        void onExit(World& w, GameStateArguments&) final override;

        void update(const vector<Event>&) final override;
    protected:
    private:
        GameManager* _game;
        sfg::Window::Ptr _start_menu;
        sfg::Button::Ptr _start_button;
};

}
#endif // TITLESCREENSTATE_HPP
