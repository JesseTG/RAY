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
#include <Box2D/Box2D.h>

#include "ResourceManager.hpp"

namespace ray {
using std::shared_ptr;
using std::make_shared;
using std::unordered_map;
using std::make_pair;
using std::string;
using std::pair;
using std::vector;
using boost::property_tree::ptree;
using boost::regex;
using boost::regex_search;
using boost::regex_match;
using boost::match_results;
using boost::smatch;
using sf::Shape;
using sf::Drawable;
using sf::Transformable;
using sf::Color;
using sf::CircleShape;
using sf::RectangleShape;
using sf::ConvexShape;
using sf::Vector2f;
using thor::ConcaveShape;

/**
 * A structure containing the resulting graphics of an SVG file and a collision
 * volume, if any.
 */
struct GameShape {
    vector<shared_ptr<Drawable>> graphics_shapes;
    shared_ptr<b2Shape> physics_shape;
};

/**
 * Loads SVG files and converts them into graphics and Box2D collision shapes.
 */
class ShapeManager : public ResourceManager
{
    public:
        ShapeManager();
        virtual ~ShapeManager();

        bool loadConfigFile(const string& path) override;
        GameShape makeShape(const string& id) {
            if (this->_shapes[id].second.graphics_shapes.empty()) {
                this->_shapes[id].second = this->_parse_shape(id, this->_shapes[id].first.get_child("svg"));
            }

            return this->_shapes[id].second;
        }
    private:
        /**
         * Represents common style attributes stored within the SVG attribute "style".
         */
        struct Style {
            Style(const Color& stroke, const float stroke_width, const Color& fill) :
                stroke(stroke),
                stroke_width(stroke_width),
                fill(fill)
            {}

            Style() :
                stroke(Color::Transparent),
                stroke_width(1.0),
                fill(Color::Black)
            {}

            /**
             * Corresponds to the @c stroke and @c stroke-opacity attributes.
             */
            Color stroke;

            /**
             * Corresponds to the @c stroke-width attribute.
             */
            float stroke_width;

            /**
             * Corresponds to the @c fill and @c fill-opacity attributes.
             */
            Color fill;
        };

        unordered_map<string, pair<ptree, GameShape>> _shapes;
        GameShape _parse_shape(const string& name, const ptree& xml);

        void _throw_if_wrong_element(const ptree& xml, const string& element);

        shared_ptr<Shape> _parse_circle(const ptree& xml);
        shared_ptr<Shape> _parse_ellipse(const ptree& xml);
        shared_ptr<Shape> _parse_line(const ptree& xml);
        shared_ptr<Shape> _parse_path(const ptree& xml);
        shared_ptr<Drawable> _parse_polygon(const ptree& xml);
        shared_ptr<Drawable> _parse_polyline(const ptree& xml);
        shared_ptr<Shape> _parse_rect(const ptree& xml);
        vector<Vector2f> _parse_points(const ptree& xml);
        Style _parse_style(const ptree& xml);

        /**
         * Given a string representing a CSS color, figure out exactly what
         * form it is and return a @c sf::Color based on it.
         */
        const Color _parse_color(const string& text);

        static unordered_map<string, const Color> NAMED_COLORS;
        static const string HEX_COLOR_STRING;
        static const string RGB_COLOR_STRING;
        static const string RGB_PERCENT_COLOR_STRING;
        static const string COLOR_STRING;
        static const string WORD_COLOR_STRING;
        static const string UNIT_STRING;
        static const string DECIMAL_STRING;
        static const string FILL_STRING;
        static const string FILL_OPACITY_STRING;
        static const string STROKE_STRING;
        static const string STROKE_OPACITY_STRING;
        static const string STROKE_WIDTH_STRING;
        static const string OPTIONAL_COMMA_STRING;

        static const regex HEX_COLOR_REGEX;
        static const regex RGB_COLOR_REGEX;
        static const regex RGB_PERCENT_COLOR_REGEX;
        static const regex WORD_COLOR_REGEX;
        static const regex UNIT_REGEX;
        static const regex DECIMAL_REGEX;
        static const regex COLOR_REGEX;
        static const regex FILL_REGEX;
        static const regex FILL_OPACITY_REGEX;
        static const regex STROKE_REGEX;
        static const regex STROKE_OPACITY_REGEX;
        static const regex STROKE_WIDTH_REGEX;
        static const regex OPTIONAL_COMMA_REGEX;

};


}

#endif // SHAPEMANAGER_HPP
