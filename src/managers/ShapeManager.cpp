#include "ShapeManager.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <sstream>
#include <string>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <Thor/Vectors.hpp>

#include "util.hpp"

namespace ray {
using std::array;
using std::count;
using std::stof;
using std::max;

using boost::trim_copy;
using boost::to_lower_copy;
using boost::regex;
using boost::regex_match;
using boost::ssub_match;
using boost::smatch;

const string ShapeManager::HEX_COLOR_STRING =
    "(?:#)([0-9a-fA-f]{6})|(?:#)([0-9a-fA-f]{3})";

const string ShapeManager::RGB_COLOR_STRING =
    "rgb(?:\\s*)?\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)";

const string ShapeManager::RGB_PERCENT_COLOR_STRING =
    "rgb\\s*\\(\\s*(\\d{1,3})\\s*%\\s*,\\s*(\\d{1,3})\\s*%\\s*,\\s*(\\d{1,3})\\s*%\\s*\\)";

const string ShapeManager::WORD_COLOR_STRING =
    "\\w+";

const string ShapeManager::UNIT_STRING =
    "e[mx]|p[ctx]|in|[cm]m";

const string ShapeManager::DECIMAL_STRING =
    "\\d*\\.\\d+|\\d+(\\.\\d*)?";

const string ShapeManager::COLOR_STRING =
    ShapeManager::HEX_COLOR_STRING + "|"
    + ShapeManager::RGB_COLOR_STRING + "|"
    + ShapeManager::RGB_PERCENT_COLOR_STRING + "|"
    + WORD_COLOR_STRING;

const string ShapeManager::FILL_STRING =
    "fill\\s*:\\s*(" + COLOR_STRING + ")\\s*;";

const string ShapeManager::FILL_OPACITY_STRING =
    "fill-opacity\\s*:\\s*(" + DECIMAL_STRING + ")\\s*;";

const string ShapeManager::STROKE_STRING =
    "stroke\\s*:\\s*(" + COLOR_STRING + ")\\s*;";

const string ShapeManager::STROKE_OPACITY_STRING =
    "stroke-opacity\\s*:\\s*(" + DECIMAL_STRING + ")\\s*;";

const string ShapeManager::STROKE_WIDTH_STRING =
    "stroke-width\\s*:\\s*(" + DECIMAL_STRING + ")?\\s*(" + UNIT_STRING + ")\\s*;";

const string ShapeManager::OPTIONAL_COMMA_STRING =
    "\\s*,\\s*|\\s+,?\\s*";

const regex ShapeManager::HEX_COLOR_REGEX(ShapeManager::HEX_COLOR_STRING);
const regex ShapeManager::RGB_COLOR_REGEX(ShapeManager::RGB_COLOR_STRING);
const regex ShapeManager::RGB_PERCENT_COLOR_REGEX(ShapeManager::RGB_PERCENT_COLOR_STRING);
const regex ShapeManager::WORD_COLOR_REGEX(ShapeManager::WORD_COLOR_STRING);
const regex ShapeManager::UNIT_REGEX(ShapeManager::UNIT_STRING);
const regex ShapeManager::DECIMAL_REGEX(ShapeManager::DECIMAL_STRING);
const regex ShapeManager::COLOR_REGEX(ShapeManager::COLOR_STRING);
const regex ShapeManager::FILL_REGEX(ShapeManager::FILL_STRING);
const regex ShapeManager::FILL_OPACITY_REGEX(ShapeManager::FILL_OPACITY_STRING);
const regex ShapeManager::STROKE_REGEX(ShapeManager::STROKE_STRING);
const regex ShapeManager::STROKE_OPACITY_REGEX(ShapeManager::STROKE_OPACITY_STRING);
const regex ShapeManager::STROKE_WIDTH_REGEX(ShapeManager::STROKE_WIDTH_STRING);
const regex ShapeManager::OPTIONAL_COMMA_REGEX(ShapeManager::OPTIONAL_COMMA_STRING);

ShapeManager::ShapeManager()
{
    //ctor
}

ShapeManager::~ShapeManager()
{
    //dtor
}

bool ShapeManager::loadConfigFile(const string& path) {
    this->_shapes.clear();
    ptree config;
    boost::property_tree::read_json(path, config); // TODO: Handle exception

    for (const auto& i : config.get_child("files")) {
        ptree xml;
        boost::property_tree::read_xml(i.second.data(), xml);
        this->_shapes[i.first] = make_pair(xml, GameShape());
    }

    return false;
    // TODO: Improve error handling
}

GameShape ShapeManager::_parse_shape(const string& name, const ptree& xml) {
    GameShape shape;
    for (const auto& i : xml) {
        std::cout << i.first << " " << i.second.data() << std::endl;
        if (i.first == "circle") {
            // If this is a circle...
            shape.graphics_shapes.push_back(_parse_circle(i.second));
        }
        else if (i.first == "rect") {
            // If this is a rectangle...
            shape.graphics_shapes.push_back(_parse_rect(i.second));
        }
        else if (i.first == "polygon") {
            // If this is a polygon...
            shape.graphics_shapes.push_back(_parse_polygon(i.second));
        }
    }

    return shape;
}

ShapeManager::Style ShapeManager::_parse_style(const ptree& xml) {
    auto first_match = [](smatch& m) {
        for (int i = 1; i < m.size(); ++i) {
            if (m[i].matched) return m[i];
        }
    };

    smatch attribute;
    Color fill = regex_search(xml.data(), attribute, FILL_REGEX) ?
                 _parse_color(first_match(attribute)) :
                 Color::Black; // Unspecified color means black

    float fill_opacity = regex_search(xml.data(), attribute, FILL_OPACITY_REGEX) ?
                         constrain(stof(first_match(attribute)), 0.f, 1.f) :
                         1.f; // Unspecified fill opacity means fully opaque

    Color stroke = regex_search(xml.data(), attribute, STROKE_REGEX) ?
                   _parse_color(first_match(attribute)) :
                   Color::Transparent; // No stroke color means no stroke

    float stroke_width = regex_search(xml.data(), attribute, STROKE_WIDTH_REGEX) ?
                         stof(first_match(attribute)) :
                         1.f; // Unspecified stroke width means one unit

    float stroke_opacity = regex_search(xml.data(), attribute, STROKE_OPACITY_REGEX) ?
                           constrain(stof(first_match(attribute)), 0.f, 1.f) :
                           1.f; // Unspecified stroke opacity means fully opaque

    fill.a = constrain(int(fill_opacity * 255.f), 0, 255);
    stroke.a = constrain(int(stroke_opacity * 255.f), 0, 255);
    return Style(stroke, stroke_width, fill);
}

shared_ptr<Shape> ShapeManager::_parse_circle(const ptree& xml) {
    // Assume that xml represents a <circle> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "circle");
#endif // DEBUG

    const auto& circle = xml.get_child_optional("<xmlattr>");
    if (!circle) {
        // If we see a blank <circle/> element without attributes...
        shared_ptr<CircleShape> shape = make_shared<CircleShape>(0.0f);
        shape->setFillColor(Color::Black);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0.0);

        return shape;
    }
    else {
        float r = max(stof(circle->get("r", "0")), 0.0f);
        // Negative or invalid radii means no visible circle

        float cx = stof(circle->get("cx", "0"));
        // Unspecified x-center means 0

        float cy = stof(circle->get("cy", "0"));
        // Unspecified y-center means 0

        Style style = _parse_style(circle->get_child("style"));

        shared_ptr<CircleShape> shape = make_shared<CircleShape>(r);
        shape->setFillColor(style.fill);
        shape->setOutlineColor(style.stroke);
        shape->setOutlineThickness(style.stroke_width);
        shape->setPosition(cx - r, cy - r);

        return shape;
    }
}

shared_ptr<Shape> ShapeManager::_parse_rect(const ptree& xml) {
    // Assume that xml represents a <rect> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "rect");
#endif // DEBUG

    const auto& rect = xml.get_child_optional("<xmlattr>");
    if (!rect) {
        // If we see a blank <rect/> element without attributes...
        shared_ptr<RectangleShape> shape = make_shared<RectangleShape>();
        shape->setFillColor(Color::Black);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0.0);

        return shape;
    }
    else {
        Vector2f pos(stof(rect->get("x", "0")), stof(rect->get("y", "0")));
        Vector2f size(stof(rect->get("width", "0")), stof(rect->get("height", "0")));
        Vector2f radius(stof(rect->get("rx", "0")), stof(rect->get("ry", "0")));
        // Unspecified coordinates default to zero

        Style style = _parse_style(rect->get_child("style"));

        shared_ptr<Shape> shape;
        int average = int(radius.x + radius.y) / 2;
        if (average == 0) {
            shape = make_shared<RectangleShape>(size);
            shape->setFillColor(style.fill);
            shape->setOutlineColor(style.stroke);
            shape->setOutlineThickness(style.stroke_width);
        }
        else {
            // SVG supports two dimensions of rounded rectangle corners, while
            // thor::Shapes::roundedRect only supports one. As a compromise,
            // I set them to the average of rx and ry.
            shape = make_shared<ConvexShape>(
                        thor::Shapes::roundedRect(
                            size, average, style.fill, style.stroke_width, style.stroke
                        )
                    );
        }

        shape->setPosition(pos);
        return shape;
    }
}

shared_ptr<Shape> ShapeManager::_parse_ellipse(const ptree& xml) {
    return nullptr;
}

shared_ptr<Shape> ShapeManager::_parse_line(const ptree& xml) {
    // Assume that xml represents a <line> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "line");
#endif // DEBUG

    const auto& line = xml.get_child_optional("<xmlattr>");
    if (!line) {
        // If we see a blank <line/> element without attributes...
        shared_ptr<ConvexShape> shape = make_shared<ConvexShape>(thor::Shapes::line(sf::Vector2f(0, 0), Color::Transparent));

        return shape;
    }
    else {
        Vector2f v1(stof(line->get("x1", "0")), stof(line->get("y1", "0")));
        Vector2f v2(stof(line->get("x2", "0")), stof(line->get("y2", "0")));
        // Unspecified coordinates default to zero

        Color stroke = _parse_color(line->get("stroke", "transparent"));
        // Unspecified stroke color means no stroke

        float stroke_width = stof(line->get("stroke-width", "1.0"));
        // Unspecified stroke width is one unit

        float stroke_opacity = constrain(stof(line->get("stroke-opacity", "1.0")), 0.0f, 1.0f);
        // Unspecified stroke opacity means fully opaque

        stroke.a = stroke_opacity * 255;

        return make_shared<ConvexShape>(thor::Shapes::line(v1, stroke, stroke_width));
        // TODO: May not be in the correct position
    }
}

shared_ptr<Shape> ShapeManager::_parse_path(const ptree& xml) {
    return nullptr;
}

/// returns either a sf::ConvexShape if the given polygon is convex or a
/// thor::ConcaveShape if it's concave
shared_ptr<Drawable> ShapeManager::_parse_polygon(const ptree& xml) {
    // Assume that xml represents a <polygon> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "polygon");
#endif // DEBUG

    const auto& polygon = xml.get_child_optional("<xmlattr>");
    if (!polygon) {
        // If we see a blank <polygon/> element without attributes...
        shared_ptr<ConvexShape> shape = make_shared<ConvexShape>();
        shape->setFillColor(Color::Black);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0.0);

        return shape;
    }
    else {
        Style style = _parse_style(polygon->get_child("style"));
        vector<Vector2f> points = _parse_points(polygon->get_child("points"));

        bool convex = true;
        float startsign = sign(thor::crossProduct(points[0] - points[1], points[1] - points[2]).z);
        for (int i = 1; i < points.size() - 1; ++i) {
            // For each point...
            Vector2f edgea(points[i - 1] - points[i]);
            Vector2f edgeb(points[i] - points[(i + 1) % points.size()]);
            // Take the sign of the cross product of the two neighboring vectors
            // If the polygon is convex, they should all be the same

            if (sign(thor::crossProduct(edgea, edgeb).z) != startsign) {
                // If the internal angle here is over 180 degrees...
                convex = false;
                break;
            }
        }

        if (convex) {
            // TODO: This code below is the same minus the type, as
            // thor::ConcaveShape doesn't extend sf::Shape.  I have no idea why.
            // I should put it in a template.
            shared_ptr<ConvexShape> shape = make_shared<ConvexShape>();
            shape->setPointCount(points.size());

            for (int i = 0; i < points.size(); ++i) {
                shape->setPoint(i, points[i]);
            }

            shape->setFillColor(style.fill);
            shape->setOutlineColor(style.stroke);
            shape->setOutlineThickness(style.stroke_width);

            return shape;
        }
        else {
            shared_ptr<ConcaveShape> shape = make_shared<ConcaveShape>();
            shape->setPointCount(points.size());

            for (int i = 0; i < points.size(); ++i) {
                shape->setPoint(i, points[i]);
            }

            shape->setFillColor(style.fill);
            shape->setOutlineColor(style.stroke);
            shape->setOutlineThickness(style.stroke_width);

            return shape;
        }
    }
}

shared_ptr<Drawable> ShapeManager::_parse_polyline(const ptree& xml) {
    return nullptr;
}

const Color ShapeManager::_parse_color(const string& text) {
    smatch match;

    const string colortext = to_lower_copy(trim_copy(text));
    if (NAMED_COLORS.count(colortext)) {
        // If this is a predefined color...
        return NAMED_COLORS[colortext];
    }
    else if (regex_search(text, match, HEX_COLOR_REGEX)) {
        // If the given color is in hexadecimal notation...
        for (int i = 1; i < match.size(); ++i) {
            // For every match but the first (which is the string itself...)
            switch (match[i].length()) {
                case 3:
                    // 3 hex digits
                    {
                        int color = stoi(match.str(i), nullptr, 16);
                        return Color(
                                   ((color & 0xF00) >> 8) * 17, // a{16} * 17{10} = aa{16}
                                   ((color & 0x0F0) >> 4) * 17,
                                   ((color & 0x00F) * 17)
                               );
                    }
                case 6:
                    // 6 hex digits
                    {
                        int color = stoi(match.str(i), nullptr, 16);
                        return Color(
                                   (color & 0x00FF0000) >> 16, // red
                                   (color & 0x0000FF00) >> 8, // green
                                   color & 0x000000FF // blue
                               );
                    }
                default:
                    // Not a valid match
                    ;
            }
        }
        throw std::logic_error("Could not find a color match in search string " + text);
        // At this point, we should've found a match
    }
    else if (regex_search(text, match, RGB_COLOR_REGEX)) {
        array<int, 3> rgb;
        for (int i = 1, array_index = 0; i < match.size(); ++i) {
            // For every match but the first (which is the string itself...)
            if (match[i].matched) {
                // If this is an RGB component...
                rgb[array_index] = constrain(stoi(match[i]), 0, 255);
                array_index++;
            }
        }
        return Color(rgb[0], rgb[1], rgb[2]);
    }
    else if (regex_search(text, match, RGB_PERCENT_COLOR_REGEX)) {
        array<float, 3> rgb;
        for (int i = 1, array_index = 0; i < match.size(); ++i) {
            // For every match but the first (which is the string itself...)
            if (match[i].matched) {
                // If this is an RGB component...
                rgb[array_index] = constrain(stof(match[i]) / 100.f, 0.f, 1.f);
                array_index++;
            }
        }
        return Color(rgb[0] * 255, rgb[1] * 255, rgb[2] * 255);
    }
    return Color::Black;
}

vector<Vector2f> ShapeManager::_parse_points(const ptree& xml) {
    vector<Vector2f> points;
    vector<string> points_str;
    boost::algorithm::split_regex(points_str, xml.data(), OPTIONAL_COMMA_REGEX);

    if (points_str.size() % 2 == 1) {
        // If we were given an odd number of coordinates...
        std::ostringstream err;
        err << "Expected an odd number of numbers, got " << points_str.size();
        throw std::invalid_argument(err.str());
    }

    for (int i = 0; i < points_str.size(); i += 2) {
        points.push_back(Vector2f(stof(points_str[i]), stof(points_str[i+1])));
    }
    return points;
}

void ShapeManager::_throw_if_wrong_element(const ptree& xml, const string& element) {
    return;
#ifdef DEBUG
    if (xml.data() != element) {
        std::ostringstream err;
        err << "Expected a <" << element << "/> element, got " << xml.data();
        throw std::invalid_argument(err.str());
    }
#endif // DEBUG
}

}

