#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include <anax/anax.hpp>
#include <SFML/Graphics.hpp>

#include "components.hpp"

#include "managers.hpp"

namespace ray {

using anax::ComponentFilter;
using anax::Entity;
using anax::System;

using sf::RenderWindow;
using sf::Transformable;

class RenderSystem : public System<RenderSystem>
{
    public:
        RenderSystem(GameManager&);
        void update();
    private:
		GameManager* _gm;
        sf::View _view;

		static bool _sort_entities(const Entity&, const Entity&) noexcept;
		const static ComponentFilter FILTER;
};
}
#endif // RENDERSYSTEM_HPP
