#ifndef TRACTORBEAMSYSTEM_HPP
#define TRACTORBEAMSYSTEM_HPP

#include <anax/anax.hpp>
#include <SFML/Graphics.hpp>

namespace ray {
using sf::RenderWindow;
using anax::Entity;
using anax::ComponentFilter;

class TractorBeamSystem : public anax::System<TractorBeamSystem>
{
    public:
        TractorBeamSystem(const Entity&, RenderWindow&);
        ~TractorBeamSystem();
        void update();
    private:
        RenderWindow* _window;
        Entity _entity;

        static const ComponentFilter FILTER;
};
}

#endif // TRACTORBEAMSYSTEM_HPP
