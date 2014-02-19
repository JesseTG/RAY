#include "entities.hpp"
#include "components.hpp"

namespace ray {
namespace entities {

World*        _world ;
RenderWindow* _window;

void setWorld(World& world) noexcept {
    _world = &world;
}

void setRenderWindow(RenderWindow& window) noexcept {
    _window = &window;
}

Entity createKeyboardCircle(float r, float x, float y) noexcept {
    Entity e = _world->createEntity();
    CircleShape* circle = new CircleShape(r, 30);
    circle->setFillColor(Color::Red);
    circle->setPosition(x, y);

    e.addComponent(new RenderableComponent(circle));
    e.addComponent(new PositionComponent(x, y));
    e.addComponent(new VelocityComponent);
    e.addComponent(new FourWayControlComponent);
    _world->activateEntity(e);
    return e;
}

Entity createMouseCircle(float r) noexcept {
    Entity e = _world->createEntity();
    CircleShape* circle = new CircleShape(r);

    circle->setOutlineThickness(1);
    circle->setOutlineColor(Color::White);
    circle->setFillColor(Color::Transparent);
    circle->setOrigin(r, r);

    e.addComponent(new RenderableComponent(circle));
    e.addComponent(new PositionComponent);
    e.addComponent(new MouseFollowControlComponent);
    _world->activateEntity(e);

    return e;
}

}
}
