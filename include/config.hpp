#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
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
 * The number of iterations the Box2D physics solver will run over velocity for
 * each step
 */
const int VELOCITY_ITERATIONS = 8;

/**
 * The number of iterations the Box2D physics solver will run over position for
 * each step
 */
const int POSITION_ITERATIONS = 3;

/**
 * It's pi.  What did you expect?
 */
const double PI = 3.141592653589793;
}


#endif // CONFIG_HPP
