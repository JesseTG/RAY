#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/System.hpp>

namespace ray {
/**
 * Width of the game resolution, in pixels
 */
const int SCREEN_WIDTH = 800;

/**
 * Height of the game resolution, in pixels
 */
const int SCREEN_HEIGHT = 600;

/**
 * A @c sf::Vector2 that holds the width and height of the screen in pixels.
 */
const sf::Vector2i SCREEN_SIZE(SCREEN_WIDTH, SCREEN_HEIGHT);

/**
 * The desired framerate of the game
 */
const int FPS = 60;

/**
 * The desired period of a frame, in seconds
 */
const float SPF = 1.0 / FPS;

/**
 * The number of iterations the Box2D physics solver will run over velocity
 */
const int VELOCITY_ITERATIONS = 8;

/**
 * The number of iterations the Box2D physics solver will run over position
 */
const int POSITION_ITERATIONS = 3;
}


#endif // CONFIG_HPP
