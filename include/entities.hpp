#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <functional>
#include <fstream>
#include <string>
#include <sstream>
#include <type_traits>


#include <SFML/Graphics.hpp>
#include <anax/anax.hpp>
#include <Box2D/Box2D.h>
#include <LuaContext.hpp>

#include "util.hpp"

namespace ray {
namespace entities {
using std::function;
using std::string;

using anax::Entity;
using anax::World;

using sf::CircleShape;
using sf::ConvexShape;
using sf::Color;
using sf::RectangleShape;
using sf::RenderWindow;
using sf::Vector2;
using sf::Vector2i;
using sf::Vector2f;
using sf::Vector2u;

extern World*        _world ;
extern RenderWindow* _window;
extern b2World*      _physics_world;
extern LuaContext*    _lua;

extern b2BodyDef ENEMY_BODY;
extern b2FixtureDef ENEMY_FIXTURE;
extern b2BodyDef PLAYER_BODY;
extern b2FixtureDef PLAYER_FIXTURE;
extern b2FixtureDef TRACTOR_BEAM_FIXTURE;

extern b2CircleShape PLAYER_SHAPE;
extern b2PolygonShape TRACTOR_BEAM_SHAPE;

/**
 * Sets the @c World that will create the @c Entities (so you don't have to
 * keep passing in the @c World as a parameter to each factory function).
 *
 * @param world The @c World that created @c Entities will belong to
 */
void setWorld(World&) noexcept;

/**
 * Sets the @c RenderWindow that will ultimately be rendered to so we don't
 * have to keep passing it into certain factory functions, as sometimes
 * information provided by a @c RenderWindow, e.g. mouse position or a @c View
 * offset, may be necessary.
 *
 * @param window The @c RenderWindow used for this game.
 */
void setRenderWindow(RenderWindow&) noexcept;

void setPhysicsWorld(b2World&) noexcept;

void setLuaState(LuaContext& lua) noexcept;

/**
 * Initializes Box2D body definitions
 */
void initBodyDefs() noexcept;

/**
 * Creates and activates circle that can be controlled by the arrow keys. More
 * specifically, creates an @c Entity with the following components:
 *
 * @li RenderableComponent
 * @li PositionComponent
 * @li VelocityComponent
 * @li FourWayControlComponent
 *
 * @param r The radius of the @c Entity in pixels
 * @param x The horizontal position of the @c Entity in the @c World in pixels
 * @param y The vertical position of the @c Entity in the @c World in pixels
 *
 * @return The new @c Entity
 */
Entity createKeyboardCircle(const Entity& face_entity, const float r, const float x, const float y) noexcept;

/**
 * Creates and activates a crosshair (in the form of a circle) that follows the
 * mouse. More specifically, creates an @c Entity with the following components:
 *
 * @li RenderableComponent
 * @li PositionComponent
 * @li MouseFollowComponent
 *
 * @param r The radius, in pixels, of the crosshair
 *
 * @return The new @c Entity
 */
Entity createMouseCircle(const float r) noexcept;

/**
 * Creates and activates a tractor beam
 */
Entity createTractorBeam(
    const Entity&,
    const Entity&,
    const float,
    const float,
    const float,
    const float
) noexcept;

/**
 * Creates an Entity with the given name
 */
template<typename...Types>
Entity createEntity(const string& type, Types...args) {
    std::ifstream in("data/script/entities.lua");
    _lua->executeCode(in);
    string name = string("create_Entity_") + type;
    std::function<Entity(Types...)> f =
        _lua->readVariable<std::function<Entity(Types...)>>(name);
    return f(args...);
}

void initBaseTypes();

void initComponentLuaBindings();

template<class SFDrawableT>
void initCommonSFMLDrawableBindings(const string& name) {

    _lua->registerMember<SFDrawableT, Vector2f>("position",
    [](const SFDrawableT& s) {
        return s.getPosition();
    },
    [](SFDrawableT& s, const Vector2f& position) {
        s.setPosition(position);
    });
    _lua->registerMember<SFDrawableT, float>("rotation",
    [](const SFDrawableT& s) {
        return s.getRotation();
    },
    [](SFDrawableT& s, const float rotation) {
        s.setRotation(rotation);
    });
    _lua->registerMember<SFDrawableT, Vector2f>("origin",
    [](const SFDrawableT& s) {
        return s.getOrigin();
    },
    [](SFDrawableT& s, const Vector2f& origin) {
        s.setOrigin(origin);
    });
    _lua->registerMember<SFDrawableT, Vector2f>("scale",
    [](const SFDrawableT& s) {
        return s.getScale();
    },
    [](SFDrawableT& s, const Vector2f& scale) {
        s.setScale(scale);
    });
    _lua->registerFunction<void(SFDrawableT::*)(const Vector2f&)>("move", &SFDrawableT::move);
    _lua->registerFunction<void(SFDrawableT::*)(const Vector2f&)>("scale", &SFDrawableT::scale);
    _lua->registerFunction("rotate", &SFDrawableT::rotate);
}

template<class SFShapeT>
void initCommonSFMLShapeBindings(const string& name) {

    _lua->registerMember<SFShapeT, Color>("fillColor",
    [](const SFShapeT& s) {
        return s.getFillColor();
    },
    [](SFShapeT& s, const Color& color) {
        s.setFillColor(color);
    });
    _lua->registerMember<SFShapeT, Color>("outLineColor",
    [](const SFShapeT& s) {
        return s.getOutlineColor();
    },
    [](SFShapeT& s, const Color& color) {
        s.setOutlineColor(color);
    });
    _lua->registerMember<SFShapeT, float>("outlineThickness",
    [](const SFShapeT& s) {
        return s.getOutlineThickness();
    },
    [](SFShapeT& s, const float thickness) {
        s.setOutlineThickness(thickness);
    });
}

}

}


#endif // ENTITIES_HPP
