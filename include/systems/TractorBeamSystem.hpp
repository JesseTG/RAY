#ifndef TRACTORBEAMSYSTEM_HPP
#define TRACTORBEAMSYSTEM_HPP

#include <anax/anax.hpp>
#include <SFML/Graphics.hpp>

#include "listeners.hpp"

namespace ray {
using sf::RenderWindow;
using sf::Mouse;
using sf::Shape;
using sf::Color;

using anax::Entity;
using anax::ComponentFilter;

class TractorBeamSystem : public anax::System<TractorBeamSystem>
{
    public:
        TractorBeamSystem(TractorBeamRepellingListener&, Entity&);
        ~TractorBeamSystem();
        void update();
    private:
        TractorBeamRepellingListener* _listener;
        Entity* _player_beam;

        static const ComponentFilter FILTER;
};
}

#endif // TRACTORBEAMSYSTEM_HPP
