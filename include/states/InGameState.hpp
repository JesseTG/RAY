#ifndef INGAMESTATE_HPP
#define INGAMESTATE_HPP

#include <vector>
#include <list>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFGUI/SFGUI.hpp>
#include <anax/anax.hpp>

#include "fsm.hpp"
#include "managers.hpp"
#include "listeners.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "Constants.hpp"

namespace ray {
using std::vector;
using std::list;
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
        int _lives;
        int _quota;
        function<void(string)> _load_level;
        MultiContactListener contact_listener;
        TractorBeamRepellingListener tb_listener;
        CollisionDamageListener damage_listener;
        FourWayControlSystem four_way_movement;
        RenderSystem rendering;
        AISystem ai;
        MovementSystem movement;
        DeathSystem death;
        MouseFollowControlSystem mouse_following;
        FaceEntitySystem face_entity;
        EntityFollowSystem follow_entity;
        TractorBeamSystem tractor_system;
        PhysicsSystem physics;
        TimerSystem timer;
        RemovalSystem removal;
        function<Entity(int)> getSpawningPoint;
        function<int(int)> getSpawningFreq;
        int spawningAmt;
        list<Entity> spawningPoints;
        list<int> spawningFreqs;
        list<int> spawningTimers;

#ifdef DEBUG
        DebugSystem debug;
#endif // DEBUG
};
}

#endif // INGAMESTATE_HPP
