#include "systems/RenderSystem.hpp"

#include <algorithm>
#include <iostream>

namespace ray {

using std::sort;

const ComponentFilter RenderSystem::FILTER = ComponentFilter()
        .requires<RenderableComponent, PositionComponent>();

RenderSystem::RenderSystem(GameManager& gm) : Base(FILTER), _gm(&gm)
{
    // Initialize the view to a rectangle located at (0, 0) and with a size of 400x200
    _view.reset(
        sf::FloatRect(
            0,
            0,
            gm.getRenderWindow()->getSize().x,
            gm.getRenderWindow()->getSize().y
        )
    );
    // Set its target viewport to be half of the window
    _view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    //testing scrolling
    //_view->setCenter(0, 0);
    gm.getRenderWindow()->setView(_view);
}

void RenderSystem::update() {
    auto entities = this->getEntities();
    RenderWindow& window = *this->_gm->getRenderWindow();
    sort(entities.begin(), entities.end(), &RenderSystem::_sort_entities);

    window.clear();
    const Vector2f playerPosVect = (_gm->getPlayer().getComponent<PositionComponent>()).position;
    _view.setCenter(playerPosVect.x, playerPosVect.y);
    window.setView(_view);

    for (Entity& e : entities) {
        RenderableComponent& graphic = e.getComponent<RenderableComponent>();
        PositionComponent&   p       = e.getComponent<PositionComponent>();

        if (graphic.transformable) {
            // If this render item can be transformed...
            // TODO: Figure out if SFML handles clipping under the hood
            graphic.transformable->setPosition(p.position);
        }
        window.draw(*(graphic.drawable), graphic.render_states);
    }
    _gm->getHealthBar()->SetFraction((float)(_gm->getPlayer().getComponent<HealthComponent>().health) / 100);
    _gm->getDesktop()->Update(1.0f);
    _gm->getSfgui()->Display(*_gm->getRenderWindow());
    window.display();
}

bool RenderSystem::_sort_entities(const Entity& e1, const Entity& e2) noexcept {
    RenderableComponent& r1 = e1.getComponent<RenderableComponent>();
    RenderableComponent& r2 = e2.getComponent<RenderableComponent>();

    return r1.layer < r2.layer;
}

}

