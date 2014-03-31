#ifndef SHAPEMANAGER_HPP
#define SHAPEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/regex.hpp>

#include <SFML/Graphics.hpp>
#include <Thor/Shapes.hpp>

#include "ResourceManager.hpp"

namespace ray {
using std::shared_ptr;
using std::make_shared;
using std::unordered_map;
using std::string;
using std::pair;
using std::vector;
using boost::property_tree::ptree;
using boost::regex;
using sf::Shape;
using sf::Color;
using sf::CircleShape;
using sf::RectangleShape;
using sf::ConvexShape;
using thor::ConcaveShape;

class ShapeManager : public ResourceManager
{
    public:
        ShapeManager();
        virtual ~ShapeManager();

        bool loadConfigFile(const string& path) override;
    private:
        unordered_map<string, pair<string, vector<shared_ptr<Shape>>>> _shapes;
        void _parse_shape(const ptree& xml);

        shared_ptr<Shape> _parse_circle(const ptree& xml);
        shared_ptr<Shape> _parse_ellipse(const ptree& xml);
        shared_ptr<Shape> _parse_line(const ptree& xml);
        shared_ptr<Shape> _parse_path(const ptree& xml);
        shared_ptr<Shape> _parse_polygon(const ptree& xml);
        shared_ptr<Shape> _parse_polyline(const ptree& xml);
        shared_ptr<Shape> _parse_rect(const ptree& xml);
        const Color _parse_color(const string& text);

        static unordered_map<string, const Color> NAMED_COLORS;
        static const regex HEX_COLOR;
        static const regex RGB_COLOR;
        static const regex RGB_PERCENT_COLOR;
};
}

#endif // SHAPEMANAGER_HPP
