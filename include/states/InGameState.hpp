#ifndef INGAMESTATE_HPP
#define INGAMESTATE_HPP

#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFGUI/SFGUI.hpp>
#include <anax/anax.hpp>

#include "fsm.hpp"
#include "managers.hpp"
#include "listeners.hpp"
#include "components.hpp"
#include "systems.hpp"

namespace ray {
using std::vector;
using sf::Event;
using anax::World;
using util::GameStateArguments;
class InGameState : public util::WorldState<vector<Event>>
{
    public:
        InGameState(GameManager& game);
        virtual ~InGameState();

        void onEnter(World& w, GameStateArguments&) final override;

        void onExit(World& w, GameStateArguments&) final override;

        void update(const vector<Event>& events) final override;

    private:
        GameManager* _game;
        sfg::Window::Ptr _gui;
        sfg::ProgressBar::Ptr _health_bar;
        sfg::Box::Ptr _health_box;
        TractorBeamRepellingListener tb_listener;
        FourWayControlSystem four_way_movement;
        RenderSystem rendering;
        AISystem ai;
        MovementSystem movement;
        MouseFollowControlSystem mouse_following;
        FaceEntitySystem face_entity;
        EntityFollowSystem follow_entity;
        TractorBeamSystem tractor_system;
        PhysicsSystem physics;
#ifdef DEBUG
        DebugSystem debug;
#endif // DEBUG
};
}

#endif // INGAMESTATE_HPP
