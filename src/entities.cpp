#include "entities.hpp"
#include "components.hpp"
#include "config.hpp"
#include "util.hpp"

namespace ray {
namespace entities {

World*        _world ;
RenderWindow* _window;
b2World* _physics_world;

b2BodyDef ENEMY_BODY;
b2CircleShape ENEMY_SHAPE;
b2FixtureDef ENEMY_FIXTURE;

b2BodyDef PLAYER_BODY;
b2CircleShape PLAYER_SHAPE;
b2FixtureDef PLAYER_FIXTURE;

b2PolygonShape TRACTOR_BEAM_SHAPE;
b2FixtureDef TRACTOR_BEAM_FIXTURE;

void setWorld(World& world) noexcept {
    _world = &world;
}

void setRenderWindow(RenderWindow& window) noexcept {
    _window = &window;
}

void setPhysicsWorld(b2World& world) noexcept {
    _physics_world = &world;
}

void initBodyDefs() noexcept {
    ENEMY_SHAPE.m_radius = 5.0;
    ENEMY_BODY.allowSleep = false;
    ENEMY_BODY.type = b2BodyType::b2_dynamicBody;
    ENEMY_BODY.active = true;
    ENEMY_FIXTURE.shape = &ENEMY_SHAPE;

    PLAYER_BODY.allowSleep = false;
    PLAYER_BODY.type = b2BodyType::b2_dynamicBody;
    PLAYER_BODY.active = true;

    PLAYER_FIXTURE.shape = &PLAYER_SHAPE;
    PLAYER_FIXTURE.density = 2;

    TRACTOR_BEAM_FIXTURE.density = 0;
    TRACTOR_BEAM_FIXTURE.isSensor = true;
}

Entity createKeyboardCircle(const Entity& face_entity, const float r, const float x, const float y) noexcept {
    Entity e = _world->createEntity();
    CircleShape* circle = new CircleShape(r, 8);
    circle->setFillColor(Color::Red);
    circle->setPosition(x, y);
    circle->setOrigin(r, r);
    PLAYER_BODY.position = b2Vec2(x, y);
    PLAYER_SHAPE.m_radius = r;
    b2Body* body = _physics_world->CreateBody(&PLAYER_BODY);
    b2Fixture* fixture = body->CreateFixture(&PLAYER_FIXTURE);

    e.addComponent(new RenderableComponent(circle, 50));
    e.addComponent(new PositionComponent(x, y));
    e.addComponent(new VelocityComponent);
    e.addComponent(new FourWayControlComponent);
    e.addComponent(new PhysicsBodyComponent(body, e));
    e.addComponent(new PhysicsFixtureComponent(fixture, e));
    e.addComponent(new FaceEntityComponent(face_entity));
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

    PositionComponent& targetpos = follow_target.getComponent<PositionComponent>();
    PhysicsBodyComponent& pb = follow_target.getComponent<PhysicsBodyComponent>();
    TRACTOR_BEAM_SHAPE.SetAsBox(starting_width / 2, length / 2, b2Vec2(length/2, 0), PI / 2);
    TRACTOR_BEAM_FIXTURE.shape = &TRACTOR_BEAM_SHAPE;
    b2Fixture* fixture = pb.body->CreateFixture(&TRACTOR_BEAM_FIXTURE);

    RectangleShape* rectangle = new RectangleShape(Vector2f(length, starting_width));
    rectangle->setFillColor(Color::Blue);
    rectangle->setOrigin(0.0, starting_width / 2.0);

    e.addComponent(new RenderableComponent(rectangle, 10));
    e.addComponent(new PositionComponent(targetpos));
    e.addComponent(new EntityFollowComponent(follow_target));
    e.addComponent(new FaceEntityComponent(face_target));
    e.addComponent(new PhysicsFixtureComponent(fixture, e));
    e.addComponent(new TractorBeamComponent);

    _world->activateEntity(e);
    return e;
}

Entity createEnemy(const float x, const float y, const float r) noexcept {
    Entity e = _world->createEntity();
    CircleShape* circle = new CircleShape(r, 8);
    circle->setFillColor(Color::Green);
    circle->setPosition(x, y);
    circle->setOrigin(r, r);

    ENEMY_BODY.position = b2Vec2(x, y);
    ENEMY_SHAPE.m_radius = r;
    b2Body* body = _physics_world->CreateBody(&ENEMY_BODY);
    b2Fixture* fixture = body->CreateFixture(&ENEMY_FIXTURE);

    e.addComponent(new RenderableComponent(circle, 500));
    e.addComponent(new PositionComponent(x, y));
    e.addComponent(new VelocityComponent);
    e.addComponent(new AccelerationComponent);
    e.addComponent(new TractorBeamRepellableComponent);
    e.addComponent(new PhysicsBodyComponent(body, e));
    e.addComponent(new PhysicsFixtureComponent(fixture, e));

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
