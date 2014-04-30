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
            lua.writeFunction(#ctype, "new", []() { \
                return new construct::type; \
            }); \
        } \
        auto addc = [](Entity& e, ctype* c) { e.addComponent<ctype>(c); }; \
        auto hasc = [](Entity& e) { return e.hasComponent<ctype>(); }; \
        auto getc = [](Entity& e) { return &(e.getComponent<ctype>()); }; \
        lua.registerFunction<Entity, void(ctype*)>(string("add") + #ctype, addc); \
        lua.registerFunction<Entity, bool(void)>(string("has") + #ctype, hasc); \
        lua.registerFunction<Entity, ctype*(void)>(string("get") + #ctype, getc); \
        constexpr bool init = has_static_member_function_luaInit<ctype, void(LuaContext&)>::value; \
        typedef conditional<init, ctype, LuaInitDummy> t; \
        t::type::luaInit(lua); \
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

shared_ptr<LuaContext> _lua;

struct LuaInitDummy {
    static void luaInit(LuaContext&) {}
};

void setLuaState(LuaContext& lua) noexcept {
    _lua = shared_ptr<LuaContext>(&lua);
}

void setLuaState(shared_ptr<LuaContext> lua) noexcept {
    _lua = lua;
}

void initBaseTypes(GameManager& game) {
    initAnaxTypeBindings(game);
    initSFMLTypeBindings(game);
    initBox2DTypeBindings(game);
}

void initComponentLuaBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();

    REGISTER_COMPONENT(AIComponent);
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
    REGISTER_COMPONENT(HealthComponent);
}
}
}

