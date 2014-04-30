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
#include "managers.hpp"

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

extern shared_ptr<LuaContext> _lua;

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

void initBaseTypes(GameManager& game);
void initSFMLTypeBindings(GameManager& game);
void initAnaxTypeBindings(GameManager& game);
void initBox2DTypeBindings(GameManager& game);
void initComponentLuaBindings(GameManager& game);
void initManagerTypeBindings(GameManager& game);
void initThorTypeBindings(GameManager& game);

}

}
#endif // ENTITIES_HPP
