#include "entities.hpp"

#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <memory>
#include <type_traits>
#include <unordered_map>



#include "components.hpp"
#include "config.hpp"
#include "util.hpp"
#include <LuaContext.hpp>

BOOST_TTI_HAS_STATIC_MEMBER_FUNCTION(luaInit);

/**
 * Permits a component to be seen and interacted with in Lua-space. If you want
 * to be able to manipulate its fields in Lua-space, you must add a public @c
 * static function to the relevant @c Component subclass that looks like this:
 * @code
 * void luaInit(LuaContext& lua);
 * @endcode
 * Templates are used to handle the case where this is not defined.
 * Also registers a default constructor if one normally exists, if not, you
 * have to define a Lua constructor yourself
 */
#define REGISTER_COMPONENT(ctype) \
    do { \
        \
        _lua->writeVariable(#ctype, LuaEmptyArray); \
        constexpr bool constructible = is_default_constructible<ctype>::value; \
        typedef conditional<constructible, ctype, LuaInitDummy> construct; \
        if (typeid(construct::type) == typeid(ctype)) { \
            _lua->writeFunction(#ctype, "new", []() { \
                return new construct::type; \
            }); \
        } \
        auto addc = [](Entity& e, ctype* c) { e.addComponent<ctype>(c); }; \
        _lua->registerFunction<Entity, void(ctype*)>(string("add") + #ctype, addc); \
        constexpr bool init = has_static_member_function_luaInit<ctype, void(LuaContext&)>::value; \
        typedef conditional<init, ctype, LuaInitDummy> t; \
        t::type::luaInit(*_lua); \
    } while (0)

namespace ray {
namespace entities {
using std::conditional;
using std::function;
using std::is_default_constructible;
using std::string;
using std::unordered_map;
using boost::get;
using boost::optional;
using boost::variant;
using sf::CircleShape;
using sf::Color;
using sf::ConvexShape;
using sf::Rect;
using sf::IntRect;
using sf::FloatRect;
using sf::RectangleShape;
using sf::Sprite;
using sf::Vector2;
using sf::Vector2i;
using sf::Vector2f;

World*        _world ;
RenderWindow* _window;
b2World* _physics_world;
LuaContext* _lua;

b2BodyDef ENEMY_BODY;
b2CircleShape ENEMY_SHAPE;
b2FixtureDef ENEMY_FIXTURE;

b2BodyDef PLAYER_BODY;
b2CircleShape PLAYER_SHAPE;
b2FixtureDef PLAYER_FIXTURE;

b2PolygonShape TRACTOR_BEAM_SHAPE;
b2FixtureDef TRACTOR_BEAM_FIXTURE;

struct LuaInitDummy {
    static void luaInit(LuaContext&) {}
};

void setWorld(World& world) noexcept {
    _world = &world;
}

void setRenderWindow(RenderWindow& window) noexcept {
    _window = &window;
}

void setPhysicsWorld(b2World& world) noexcept {
    _physics_world = &world;
}

void setLuaState(LuaContext& lua) noexcept {
    _lua = &lua;
}

void initBaseTypes() {
    initAnaxTypeBindings();
    initSFMLTypeBindings();
    initBox2DTypeBindings();
}

// TODO: Section this off into another compilation unit; or another project?
void initComponentLuaBindings() {
    REGISTER_COMPONENT(EntityFollowComponent);
    REGISTER_COMPONENT(FaceEntityComponent);
    REGISTER_COMPONENT(FourWayControlComponent);
    REGISTER_COMPONENT(MouseFollowControlComponent);
    REGISTER_COMPONENT(PhysicsBodyComponent);
    REGISTER_COMPONENT(PhysicsFixtureComponent);
    REGISTER_COMPONENT(PositionComponent);
    REGISTER_COMPONENT(RenderableComponent);
    REGISTER_COMPONENT(TractorBeamComponent);
    REGISTER_COMPONENT(TractorBeamRepellableComponent);
    REGISTER_COMPONENT(VelocityComponent);
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


}
}

