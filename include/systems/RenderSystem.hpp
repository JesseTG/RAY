#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include <anax/anax.hpp>
#include <SFML/Graphics.hpp>

#include "components.hpp"

namespace ray {

using anax::ComponentFilter;
using anax::Entity;
using anax::System;

using sf::RenderWindow;
using sf::Transformable;

class RenderSystem : public System<RenderSystem>
{
    public:
        RenderSystem(RenderWindow&);

        void update();
    private:
		RenderWindow* _window;
		const static ComponentFilter FILTER;
};
}
#endif // RENDERSYSTEM_HPP
