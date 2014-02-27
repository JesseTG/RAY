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

Entity createKeyboardCircle(const float r, const float x, const float y) noexcept {
    Entity e = _world->createEntity();
    CircleShape* circle = new CircleShape(r, 30);
    circle->setFillColor(Color::Red);
    circle->setPosition(x, y);
    circle->setOrigin(r, r);

    e.addComponent(new RenderableComponent(circle, 50));
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

    e.addComponent(new RenderableComponent(circle, 100));
    e.addComponent(new PositionComponent);
    e.addComponent(new MouseFollowControlComponent);
    _world->activateEntity(e);

    return e;
}

Entity createTractorBeam(
    const Entity& face_target,
    const Entity& follow_target,
    const float starting_width,
    const float starting_angle,
    const float length,
    const float force) noexcept {
    Entity e = _world->createEntity();

    RectangleShape* rectangle = new RectangleShape(Vector2f(length, starting_width));
    rectangle->setFillColor(Color::Blue);
    rectangle->setOrigin(0.0, starting_width / 2.0);

    e.addComponent(new RenderableComponent(rectangle, 10));
    e.addComponent(new PositionComponent(256, 256));
    e.addComponent(new FaceEntityComponent(face_target));
    e.addComponent(new EntityFollowComponent(follow_target));

    _world->activateEntity(e);
    return e;
}

Entity createEnemy(const float x, const float y, const float r) noexcept {
    Entity e = _world->createEntity();
    CircleShape* circle = new CircleShape(r, 8);
    circle->setFillColor(Color::Green);
    circle->setPosition(x, y);
    circle->setOrigin(r, r);

    e.addComponent(new RenderableComponent(circle, 500));
    e.addComponent(new PositionComponent(x, y));
    e.addComponent(new VelocityComponent);
    e.addComponent(new AccelerationComponent);
    e.addComponent(new TractorBeamRepellableComponent);

    _world->activateEntity(e);
    return e;
}

Entity createBullet(
    const float px,
    const float py,
    const float vx,
    const float vy) noexcept {
    Entity e = _world->createEntity();
    CircleShape* circle = new CircleShape(8);
    circle->setOrigin(8, 8);

    e.addComponent(new PositionComponent(px, py));
    e.addComponent(new RenderableComponent(circle));
    e.addComponent(new VelocityComponent(vx, vy));
    e.addComponent(new BulletComponent(2));

    _world->activateEntity(e);
    return e;
}

}
}
