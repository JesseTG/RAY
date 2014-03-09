#include "entities.hpp"

#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <memory>
#include <type_traits>
#include <unordered_map>

#include <boost/tti/has_static_member_function.hpp>

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
 */
#define REGISTER_COMPONENT(ctype) \
    do { \
        \
        _lua->writeVariable(#ctype, LuaEmptyArray); \
        _lua->writeFunction(#ctype, "new", []() { \
            return *(new ctype); \
        }); \
        auto addc = [](Entity& e, ctype& c) { e.addComponent<ctype>(&c); }; \
        _lua->registerFunction<Entity, void(ctype&)>(string("add") + #ctype, addc); \
        typedef conditional< \
        has_static_member_function_luaInit<ctype, void(LuaContext&)>::value, \
        ctype, \
        LuaInitDummy \
        > t; \
        t::type::luaInit(*_lua); \
    } while (0)

namespace ray {
namespace entities {
using std::conditional;
using std::string;
using std::unordered_map;
using sf::Color;
using sf::Rect;
using sf::IntRect;
using sf::FloatRect;
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

// TODO: Section this off into another compilation unit; or another project?
void initBaseTypes() {
    _lua->writeVariable("Anax", LuaEmptyArray);
    {
        _lua->writeVariable("Anax", "world", _world);
        {
            _lua->registerFunction("killEntity", &World::killEntity);
            _lua->registerFunction("killEntities", &World::killEntities);
            _lua->registerFunction("activateEntity", &World::activateEntity);
            _lua->registerFunction("deactivateEntity", &World::deactivateEntity);
            _lua->registerFunction("isActivated", &World::isActivated);
            _lua->registerFunction("isValid", &World::isValid);
            _lua->registerFunction("refresh", &World::refresh);
            _lua->registerFunction("clear", &World::clear);
            _lua->registerFunction("getEntityCount", &World::getEntityCount);
        }

        _lua->writeVariable("Anax", "Entity", LuaEmptyArray);
        {
            _lua->writeFunction("Anax", "Entity", "new", []() {
                Entity e = _world->createEntity();
                _world->activateEntity(e);
                return e;
            });
            _lua->registerFunction("isActivated", &Entity::isActivated);
            _lua->registerFunction("activate", &Entity::activate);
            _lua->registerFunction("deactivate", &Entity::deactivate);
            _lua->registerFunction("isValid", &Entity::isValid);
        }
    }

    _lua->writeVariable("SFML", LuaEmptyArray);
    {
        _lua->writeVariable("SFML", "Vector", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Vector", "new", getDefaultConstructorLambda<Vector2f>());
            _lua->registerMember("x", &Vector2f::x);
            _lua->registerMember("y", &Vector2f::y);
        }

        _lua->writeVariable("SFML", "Color", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Color", "new", getDefaultConstructorLambda<Color>());
            _lua->registerMember("r", &Color::r);
            _lua->registerMember("g", &Color::g);
            _lua->registerMember("b", &Color::b);
            _lua->registerMember("a", &Color::a);

            _lua->writeVariable("SFML", "Color", "Red", Color::Red);
            _lua->writeVariable("SFML", "Color", "Blue", Color::Blue);
        }

        _lua->writeVariable("SFML", "Rect", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Rect", "new", getDefaultConstructorLambda<FloatRect>());
            _lua->registerMember("left", &FloatRect::left);
            _lua->registerMember("x", &FloatRect::left);
            _lua->registerMember("top", &FloatRect::top);
            _lua->registerMember("y", &FloatRect::top);
            _lua->registerMember("width", &FloatRect::width);
            _lua->registerMember("height", &FloatRect::height);
        }
    }

    _lua->writeVariable("Box2D", LuaEmptyArray);
    {
        _lua->writeVariable("Box2D", "Vector", LuaEmptyArray);
        {
            _lua->writeFunction("Box2D", "Vector", "new", []() {
                return b2Vec2(0, 0);
            });
            _lua->registerMember("x", &b2Vec2::x);
            _lua->registerMember("y", &b2Vec2::y);
        }

        _lua->writeVariable("Box2D", "BodyType", LuaEmptyArray);
        {
            _lua->writeVariable("Box2D", "BodyType", "Static", b2_staticBody);
            _lua->writeVariable("Box2D", "BodyType", "Dynamic", b2_dynamicBody);
            _lua->writeVariable("Box2D", "BodyType", "Kinematic", b2_kinematicBody);
        }

        _lua->writeVariable("Box2D", "BodyDef", LuaEmptyArray);
        {
            _lua->writeFunction("Box2D", "BodyDef", "new", getDefaultConstructorLambda<b2BodyDef>());

            _lua->registerMember("active", &b2BodyDef::active);
            _lua->registerMember("allowSleep", &b2BodyDef::allowSleep);
            _lua->registerMember("angle", &b2BodyDef::angle);
            _lua->registerMember("angularDamping", &b2BodyDef::angularDamping);
            _lua->registerMember("angularVelocity", &b2BodyDef::angularVelocity);
            _lua->registerMember("awake", &b2BodyDef::awake);
            _lua->registerMember("bullet", &b2BodyDef::bullet);
            _lua->registerMember("fixedRotation", &b2BodyDef::fixedRotation);
            _lua->registerMember("gravityScale", &b2BodyDef::gravityScale);
            _lua->registerMember("linearDamping", &b2BodyDef::linearDamping);
            _lua->registerMember("linearVelocity", &b2BodyDef::linearVelocity);
            _lua->registerMember("position", &b2BodyDef::position);
        }

        _lua->writeVariable("Box2D", "Body", LuaEmptyArray);
        {
            _lua->registerMember<b2Body, bool>("awake",
            [](const b2Body& object) {
                return object.IsAwake();
            },
            [](b2Body& object, const bool val) {
                object.SetAwake(val);
            });
            _lua->registerMember<b2Body, bool>("fixedRotation",
            [](const b2Body& object) {
                return object.IsFixedRotation();
            },
            [](b2Body& object, const bool val) {
                object.SetFixedRotation(val);
            });
        }

        _lua->writeVariable("Box2D", "Filter", LuaEmptyArray);
        {
            _lua->writeFunction("Box2D", "Filter", "new", getDefaultConstructorLambda<b2Filter>());
            _lua->registerMember("categoryBits", &b2Filter::categoryBits);
            _lua->registerMember("maskBits", &b2Filter::maskBits);
            _lua->registerMember("groupIndex", &b2Filter::groupIndex);
        }

        _lua->writeVariable("Box2D", "FixtureDef", LuaEmptyArray);
        {
            _lua->writeFunction("Box2D", "FixtureDef", "new", getDefaultConstructorLambda<b2FixtureDef>());
            _lua->registerMember("friction", &b2FixtureDef::friction);
            _lua->registerMember("restitution", &b2FixtureDef::restitution);
            _lua->registerMember("density", &b2FixtureDef::density);
            _lua->registerMember("isSensor", &b2FixtureDef::isSensor);
            _lua->registerMember("filter", &b2FixtureDef::filter);
        }

        _lua->writeVariable("Box2D", "Fixture", LuaEmptyArray);
        {
            _lua->registerMember<b2Fixture, float>("friction",
            [](const b2Fixture& object) {
                return object.GetFriction();
            },
            [](b2Fixture& object, const float val) {
                object.SetFriction(val);
            });
            _lua->registerMember<b2Fixture, float>("restitution",
            [](const b2Fixture& object) {
                return object.GetRestitution();
            },
            [](b2Fixture& object, const float val) {
                object.SetRestitution(val);
            });
            _lua->registerMember<b2Fixture, float>("density",
            [](const b2Fixture& object) {
                return object.GetDensity();
            },
            [](b2Fixture& object, const float val) {
                object.SetDensity(val);
            });
            _lua->registerMember<b2Fixture, bool>("isSensor",
            [](const b2Fixture& object) {
                return object.IsSensor();
            },
            [](b2Fixture& object, const bool val) {
                object.SetSensor(val);
            });
        }
    }
}

void initComponentLuaBindings() {
    REGISTER_COMPONENT(AccelerationComponent);
    REGISTER_COMPONENT(EntityFollowComponent);
    REGISTER_COMPONENT(FaceEntityComponent);
    REGISTER_COMPONENT(FourWayControlComponent);
    REGISTER_COMPONENT(MouseFollowControlComponent);
    //REGISTER_COMPONENT(PhysicsBodyComponent);
    //REGISTER_COMPONENT(PhysicsFixtureComponent);
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

Entity createEntity(const string& type) {
    Entity e = _world->createEntity();
    std::ifstream in("data/script/entities.lua");
    _lua->writeVariable("entity", &e);
    _lua->executeCode(in);
    _lua->writeVariable("entity", nullptr);
    _world->activateEntity(e);
    return e;
}

}
}

