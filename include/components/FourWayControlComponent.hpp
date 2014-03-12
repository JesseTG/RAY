#ifndef FOURWAYMOVEMENTCOMPONENT_HPP
#define FOURWAYMOVEMENTCOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <anax/anax.hpp>
#include <LuaContext.hpp>

namespace ray {
using anax::Component;
using sf::Keyboard;
typedef Keyboard::Key Key;

/**
 * Enables a @c Entity to take input from four keys and move around in the
 * eight cardinal directions (the four keys plus combinations for diagonals).
 */
struct FourWayControlComponent : Component<FourWayControlComponent>
{
    /**
     * Constructs a FourWayControlComponent with the default values of
     * @c Key::W, @c Key::S, @c Key::A, and @c Key::D, respectively.
     */
    FourWayControlComponent();

    /**
     * Constructs a FourWayControlComponent that can be used with arbitrary
     * values of @c sf::Keyboard::Key.
     *
     * @param up The @c Key that will move this @c Entity up (north) when pressed
     * @param down The @c Key that will move this @c Entity down (south) when pressed
     * @param left The @c Key that will move this @c Entity left (west) when pressed
     * @param right The @c Key that will move this @c Entity right (east) when pressed
     */
    FourWayControlComponent(const Key up, const Key down, const Key left, const Key right);

    /**
     * The @c Key that will move this @c Entity up (north) when pressed
     */
    Key up;

    /**
     * The @c Key that will move this @c Entity down (south) when pressed
     */
    Key down;

    /**
     * The @c Key that will move this @c Entity left (west) when pressed
     */
    Key left;

    /**
     * The @c Key that will move this @c Entity right (east) when pressed
     */
    Key right;

    static void luaInit(LuaContext& lua);
};
}
#endif // FOURWAYMOVEMENTCOMPONENT_HPP
