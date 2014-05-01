#include "systems/RenderSystem.hpp"

#include <algorithm>
#include <iostream>
#include "util.hpp"
#include <cmath>

namespace ray {

using std::sort;

const ComponentFilter RenderSystem::FILTER = ComponentFilter()
        .requires<RenderableComponent>()
        .requiresOneOf<PhysicsBodyComponent, PhysicsFixtureComponent, PositionComponent>();

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

//TODO: Build a render list, and add a graphic to it whenever onEntityAdded is called
//No need to sort every frame that way
void RenderSystem::update() {
    auto entities = this->getEntities();
    RenderWindow& window = *this->_gm->getRenderWindow();
    sort(entities.begin(), entities.end(), &RenderSystem::_sort_entities);

    window.clear();
    const Entity& player = this->_gm->getPlayer();

    //TODO: Separate the camera to another System
    if (player.isValid() && player.hasComponent<PhysicsBodyComponent>()) {
        PhysicsBodyComponent& pbc = player.getComponent<PhysicsBodyComponent>();
        if (pbc.body) {
            const b2Vec2& pos = pbc.body->GetPosition();
            _view.setCenter(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER);
            window.setView(_view);
        }
    }

    for (Entity& e : entities) {
        RenderableComponent& graphic = e.getComponent<RenderableComponent>();
        if (e.hasComponent<PhysicsBodyComponent>()) {
            PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();
            const b2Vec2& pos = pbc.body->GetPosition();
            graphic.transformable->setPosition(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER);
            graphic.transformable->setRotation(toDegrees(pbc.body->GetAngle()));
        }
        else if (e.hasComponent<PhysicsFixtureComponent>()) {
            PhysicsFixtureComponent& pfc = e.getComponent<PhysicsFixtureComponent>();
            b2Body* body = pfc.fixture->GetBody();
            const b2Vec2& pos = body->GetPosition();
            graphic.transformable->setPosition(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER);
            graphic.transformable->setRotation(toDegrees(body->GetAngle()));
        }
        else if (e.hasComponent<PositionComponent>()) {
            PositionComponent& p = e.getComponent<PositionComponent>();
            if (graphic.transformable) {
                // If this render item can be transformed...
                // TODO: Figure out if SFML handles clipping under the hood
                graphic.transformable->setPosition(p.position * float(PIXELS_PER_METER));
            }
        }

        window.draw(*(graphic.drawable), graphic.render_states);
    }
    _gm->getDesktop()->Update(1.0f);
    _gm->getSfgui()->Display(*_gm->getRenderWindow());
    _gm->getPhysicsWorld()->DrawDebugData();
    window.display();
}

bool RenderSystem::_sort_entities(const Entity& e1, const Entity& e2) noexcept {
    RenderableComponent& r1 = e1.getComponent<RenderableComponent>();
    RenderableComponent& r2 = e2.getComponent<RenderableComponent>();

    return r1.layer < r2.layer;
}

}

