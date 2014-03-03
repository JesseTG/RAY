#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <SFML/Graphics.hpp>
#include <anax/anax.hpp>
#include <Box2D/Box2D.h>

namespace ray {
namespace entities {
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

Entity createEnemy(const float, const float, const float) noexcept;

Entity createBullet(const float, const float, const float, const float) noexcept;
}
}


#endif // ENTITIES_HPP
