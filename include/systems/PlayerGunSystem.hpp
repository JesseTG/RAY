#ifndef PLAYERGUNSYSTEM_HPP
#define PLAYERGUNSYSTEM_HPP

#include <vector>

#include <anax/anax.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace ray {

using std::vector;
using anax::ComponentFilter;
using anax::Entity;
using anax::System;

using sf::Event;
using sf::Mouse;
using sf::RenderWindow;
using sf::Vector2;
using sf::Vector2i;
using sf::Vector2f;

class PlayerGunSystem : public System<PlayerGunSystem>
{
    public:
        PlayerGunSystem(const Entity&, RenderWindow&);
        virtual ~PlayerGunSystem();
        void update(const vector<Event>&);
    private:
        Entity _player;
        RenderWindow* _window;

        static const ComponentFilter FILTER;
};
}

#endif // PLAYERGUNSYSTEM_HPP
