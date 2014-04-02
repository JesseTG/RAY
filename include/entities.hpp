#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <functional>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <type_traits>

#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <boost/tti/has_static_member_function.hpp>

#include <SFML/Graphics.hpp>
#include <anax/anax.hpp>
#include <Box2D/Box2D.h>
#include <LuaContext.hpp>

#include "util.hpp"

namespace ray {
namespace entities {
using std::ifstream;
using std::function;
using std::string;
using std::shared_ptr;

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

extern RenderWindow* _window;
extern shared_ptr<World> _world;
extern shared_ptr<b2World> _physics_world;
extern shared_ptr<LuaContext> _lua;

/**
 * Sets the @c World that will create the @c Entities (so you don't have to
 * keep passing in the @c World as a parameter to each factory function).
 *
 * @param world The @c World that created @c Entities will belong to
 */
void setWorld(World&) noexcept;

void setWorld(shared_ptr<World>) noexcept;

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

void setPhysicsWorld(b2World*) noexcept;

void setPhysicsWorld(shared_ptr<b2World>) noexcept;

void setLuaState(LuaContext&) noexcept;

void setLuaState(shared_ptr<LuaContext>) noexcept;

/**
 * Creates an @c Entity with the given name.
 *
 * Inside Lua-space, there must be a function with the name @c create_Entity_XXX,
 * where @c XXX is the actual @c Entity type (e.g. @c Cursor, @c Enemy, etc.).
 *
 * You can pass in any object type in as an argument; just make sure any code
 * within the given Lua function knows what to do with it (i.e. make sure bindings
 * exist, if necessary)!
 *
 * @tparam Types The types of any arguments passed in
 * @param type The name of the @c Entity type to create
 * @param args The arguments to be passed into the relevant Lua functio
 * @return The generated @c Entity
 */
template<typename...Types>
Entity createEntity(const string& type, Types...args) {
    string name = string("create_Entity_") + type;
    function<Entity(Types...)> f =
        _lua->readVariable<function<Entity(Types...)>>(name);
    return f(args...);
}

void initBaseTypes();
void initSFMLTypeBindings();
void initAnaxTypeBindings();
void initBox2DTypeBindings();
void initComponentLuaBindings();

}

}


#endif // ENTITIES_HPP
