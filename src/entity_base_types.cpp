#include "entities.hpp"

#include <Box2D/Box2D.h>
#include <LuaContext.hpp>
#include <SFML/Graphics.hpp>
#include <anax/anax.hpp>

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

}
}
