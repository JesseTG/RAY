#ifndef RENDERABLECOMPONENT_HPP
#define RENDERABLECOMPONENT_HPP

#include <memory>
#include <typeinfo>
#include <utility>

#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <anax/anax.hpp>
#include <SFML/Graphics.hpp>
#include <LuaContext.hpp>

namespace ray
{

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::dynamic_pointer_cast;
using std::static_pointer_cast;
using boost::variant;
using boost::get;
using anax::Component;
using sf::Drawable;
using sf::Sprite;
using sf::Text;
using sf::VertexArray;
using sf::ConvexShape;
using sf::RectangleShape;
using sf::Transformable;
using sf::CircleShape;
using sf::Transform;
using sf::Color;
using sf::RenderStates;

/**
 * Enables an @c Entity to be drawn on-screen. Typically the drawable object
 * will extend both @c sf::Drawable @em and @c sf::Transformable, but just in
 * case it stores two pointers to the same object, with different base classes
 * for performance reasons.
 */
struct RenderableComponent : Component<RenderableComponent>
{

    /**
     * Constructs a RenderableComponent with a default value of a white
     * @c sf::CircleShape with a radius of 16 pixels.
     */
    RenderableComponent() : RenderableComponent(new CircleShape(16)) {
        static_pointer_cast<CircleShape>(drawable)->setFillColor(Color::White);
    }

    /**
     * Constructs a Renderable that is initialized with the given @c sf::Drawable.
     *
     * @param d The @c sf::Drawable to represent the @c Entity this RenderableComponent
     * will belong to. If @c d extends @c sf::Transformable, @c this->transformable
     * will be initialized to point to @c d.
     */
    RenderableComponent(Drawable* d) : RenderableComponent(d, 0) {}

    RenderableComponent(shared_ptr<Drawable> d) : RenderableComponent(d, 0) {}

    /**
     * Constructs a Renderable that is initialized with the given @c sf::Drawable
     * and onto the given layer.
     *
     * @param d The @c sf::Drawable to represent the @c Entity this RenderableComponent
     * will belong to. If @c d extends @c sf::Transformable, @c this->transformable
     * will be initialized to point to @c d.
     * @param layer The number that determines when the underlying @c sf::Drawable
     * will be drawn (smaller numbers go first; negative numbers are fine).
     */
    RenderableComponent(Drawable* d, const int layer) :
        drawable(d),
        transformable(dynamic_pointer_cast<Transformable>(drawable)),
        layer(layer)
    {}

    RenderableComponent(shared_ptr<Drawable> d, const int layer) :
        drawable(d),
        transformable(dynamic_pointer_cast<Transformable>(drawable)),
        layer(layer)
    {}

    ~RenderableComponent();

    /**
     * A @c std::shared_ptr to the drawable object.
     */
    shared_ptr<Drawable> drawable;

    /**
     * A @c std::shared_ptr to @c RenderableComponent::drawable as a
     * @c sf::Transformable if @c *drawable extends @c sf::Transformable,
     * @c nullptr otherwise. We keep this pointer so that we can minimize usage
     * of @c dynamic_cast, which has some runtime overhead; do not want!
     */
    shared_ptr<Transformable> transformable;

    int layer;

    RenderStates render_states;

    static void luaInit(LuaContext& lua);
};
}

#endif // RENDERABLECOMPONENT_HPP
