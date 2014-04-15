#include "ShapeManager.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <Thor/Vectors.hpp>

#include "util.hpp"
#include "PolylineShape.hpp"
#include "EllipseShape.hpp"

namespace ray {
using std::array;
using std::count;
using std::stof;
using std::max;
using std::find;
using std::static_pointer_cast;
using std::tan;

using boost::trim_copy;
using boost::to_lower_copy;
using boost::regex;
using boost::regex_match;
using boost::ssub_match;
using boost::smatch;

using sf::PolylineShape;
using sf::EllipseShape;

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

const string ShapeManager::SIGNED_DECIMAL_STRING =
    "([\\-\\+]?(" + DECIMAL_STRING + "))";

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

const string ShapeManager::VISIBILITY_STRING =
    "visibility\\s*:\\s*(visible|hidden|collapse)\\s*;";

const string ShapeManager::NAME_STRING =
    "[^,()\\s]+";

const string ShapeManager::NAME_LIST_STRING =
    "(" + NAME_STRING + OPTIONAL_COMMA_STRING + ")*" + NAME_STRING;

const string ShapeManager::TRANSFORM_STRING =
    "(\\w+)\\s*\\(\\s*(\\-?\\s*\\d+\\.?\\d*\\s*,?\\s*)+\\)";

const string ShapeManager::TRANSFORM_TYPE_STRING =
    "[\\w\\.\\-]+";

const regex ShapeManager::HEX_COLOR_REGEX(ShapeManager::HEX_COLOR_STRING);
const regex ShapeManager::RGB_COLOR_REGEX(ShapeManager::RGB_COLOR_STRING);
const regex ShapeManager::RGB_PERCENT_COLOR_REGEX(ShapeManager::RGB_PERCENT_COLOR_STRING);
const regex ShapeManager::WORD_COLOR_REGEX(ShapeManager::WORD_COLOR_STRING);
const regex ShapeManager::UNIT_REGEX(ShapeManager::UNIT_STRING);
const regex ShapeManager::DECIMAL_REGEX(ShapeManager::DECIMAL_STRING);
const regex ShapeManager::SIGNED_DECIMAL_REGEX(ShapeManager::SIGNED_DECIMAL_STRING);
const regex ShapeManager::COLOR_REGEX(ShapeManager::COLOR_STRING);
const regex ShapeManager::FILL_REGEX(ShapeManager::FILL_STRING);
const regex ShapeManager::FILL_OPACITY_REGEX(ShapeManager::FILL_OPACITY_STRING);
const regex ShapeManager::STROKE_REGEX(ShapeManager::STROKE_STRING);
const regex ShapeManager::STROKE_OPACITY_REGEX(ShapeManager::STROKE_OPACITY_STRING);
const regex ShapeManager::STROKE_WIDTH_REGEX(ShapeManager::STROKE_WIDTH_STRING);
const regex ShapeManager::OPTIONAL_COMMA_REGEX(ShapeManager::OPTIONAL_COMMA_STRING);
const regex ShapeManager::VISIBILITY_REGEX(ShapeManager::VISIBILITY_STRING);
const regex ShapeManager::NAME_REGEX(ShapeManager::NAME_STRING);
const regex ShapeManager::NAME_LIST_REGEX(ShapeManager::NAME_LIST_STRING);
const regex ShapeManager::TRANSFORM_REGEX(ShapeManager::TRANSFORM_STRING);
const regex ShapeManager::TRANSFORM_TYPE_REGEX(ShapeManager::TRANSFORM_TYPE_STRING);

const string ShapeManager::COLLIDABLE_CLASS = "collidable";
const string ShapeManager::SOLID_CLASS = "solid";
const string ShapeManager::HOLLOW_CLASS = "hollow";

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
        pair<shared_ptr<Drawable>, shared_ptr<b2Shape>> shape_pair;
        if (i.first == "circle") {
            // If this is a circle...
            shape_pair = _parse_circle(i.second);
        }
        else if (i.first == "rect") {
            // If this is a rectangle...
            shape_pair = _parse_rect(i.second);
        }
        else if (i.first == "polygon") {
            // If this is a polygon...
            shape_pair = _parse_polygon(i.second);
        }
        else if (i.first == "line") {
            // If this is a line...
            shape_pair = _parse_line(i.second);
        }
        else if (i.first == "polyline") {
            // If this is a polyline...
            shape_pair = _parse_polyline(i.second);
        }
        else if (i.first == "ellipse") {
            // If this is an ellipse...
            shape_pair = _parse_ellipse(i.second);
        }

        if (shape_pair.first != nullptr) {
            // If we managed to load and parse a SVG element...
            shape.graphics_shapes.push_back(shape_pair.first);
            shape.physics_shapes.push_back(shape_pair.second);
        }
    }

    return shape;
}

ShapeManager::Style ShapeManager::_parse_style(const ptree& xml) {
    auto first_match = [](smatch& m) {
        for (int i = 1; i < m.size(); ++i) {
            if (m[i].matched) return m[i];
        }
        return m[0];
    };

    smatch attribute;
    Color fill = regex_search(xml.data(), attribute, FILL_REGEX) ?
                 _parse_color(first_match(attribute)) :
                 Color::Transparent; // Unspecified color means black

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

    bool visibility = regex_search(xml.data(), attribute, VISIBILITY_REGEX) ?
                      first_match(attribute) == "visibility" :
                      true; // Unspecified visibility defaults to visibility

    fill.a = constrain(int(fill_opacity * 255.f), 0, 255);
    stroke.a = constrain(int(stroke_opacity * 255.f), 0, 255);
    return Style(stroke, stroke_width, fill, visibility);
}

ShapeManager::ShapePair ShapeManager::_parse_circle(const ptree& xml) {
    // Assume that xml represents a <circle> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "circle");
#endif // DEBUG

    const auto& circle = xml.get_child_optional("<xmlattr>");
    if (!circle) {
        // If we see a blank <circle/> element without attributes...
        shared_ptr<CircleShape> shape = make_shared<CircleShape>();
        shape->setFillColor(Color::Black);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0.0);

        return make_pair(shape, nullptr);
    }
    else {
        float r = max(stof(circle->get("r", "0")), 0.0f);
        // Negative or invalid radii means no visible circle

        float cx = stof(circle->get("cx", "0"));
        // Unspecified x-center means 0

        float cy = stof(circle->get("cy", "0"));
        // Unspecified y-center means 0

        Style style = _parse_style(circle->get_child("style"));
        const auto& xml_class = circle->get_child_optional("class");
        if (xml_class) {
            style.classes = _parse_class(*xml_class);
        }

        shared_ptr<CircleShape> shape = make_shared<CircleShape>(r);
        shape->setFillColor(style.fill);
        shape->setOutlineColor(style.stroke);
        shape->setOutlineThickness(style.stroke_width);
        shape->setPosition(cx - r, cy - r);

        shared_ptr<b2CircleShape> b2shape;
        if (find(style.classes.begin(), style.classes.end(), COLLIDABLE_CLASS) != style.classes.end()) {
            // If this <circle> is marked as collidable...
            b2shape = make_shared<b2CircleShape>();
            b2shape->m_radius = r;
            b2shape->m_p = b2Vec2(cx - r, cy - r);
        }

        return make_pair(shape, b2shape);
    }
}

ShapeManager::ShapePair ShapeManager::_parse_rect(const ptree& xml) {
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

        return make_pair(shape, nullptr);
    }
    else {
        Vector2f pos(stof(rect->get("x", "0")), stof(rect->get("y", "0")));
        Vector2f size(stof(rect->get("width", "0")), stof(rect->get("height", "0")));
        Vector2f radius(stof(rect->get("rx", "0")), stof(rect->get("ry", "0")));
        // Unspecified coordinates default to zero

        Style style = _parse_style(rect->get_child("style"));
        const auto& xml_class = rect->get_child_optional("class");
        if (xml_class) {
            style.classes = _parse_class(*xml_class);
        }

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

        shared_ptr<b2Shape> ptr;
        const vector<string>& classes = style.classes;
        if (find(classes.begin(), classes.end(), COLLIDABLE_CLASS) != classes.end()) {
            // If this <rect> is collidable...
            if (find(classes.begin(), classes.end(), SOLID_CLASS) != classes.end()) {
                // If this <rect> should be a solid polygon...
                ptr = make_shared<b2PolygonShape>();
                static_pointer_cast<b2PolygonShape>(ptr)->SetAsBox(
                    size.x / 2,
                    size.y / 2,
                    b2Vec2_zero,
                    0.0
                );
            }
            else if (find(classes.begin(), classes.end(), HOLLOW_CLASS) != classes.end()) {
                // Else if this <rect> should be a chain...
                ptr = make_shared<b2ChainShape>();
                array<b2Vec2, 4> vecs = {
                    b2Vec2(pos.x, pos.y), // NW corner
                    b2Vec2(pos.x + size.x, pos.y), // NE corner
                    b2Vec2(pos.x + size.x, pos.y + size.y), // SE corner
                    b2Vec2(pos.x, pos.y + size.y) // SW corner
                };
                static_pointer_cast<b2ChainShape>(ptr)->CreateLoop(vecs.data(), vecs.size());
            }
            // Not a valid collision class, let's not make this shape collidable
        }

        shape->setPosition(pos);
        return make_pair(shape, ptr);
    }
}

// TODO: collision detection as a polygon
ShapeManager::ShapePair ShapeManager::_parse_ellipse(const ptree& xml) {
    // Assume that xml represents a <ellipse> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "ellipse");
#endif // DEBUG

    const auto& ellipse = xml.get_child_optional("<xmlattr>");
    if (!ellipse) {
        // If we see a blank <ellipse/> element without attributes...
        shared_ptr<EllipseShape> shape = make_shared<EllipseShape>();
        shape->setFillColor(Color::Black);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0.0);

        return make_pair(shape, nullptr);
    }
    else {
        float rx = max(stof(ellipse->get("rx", "0")), 0.0f);
        // Negative or invalid radii means no visible ellipse

        float ry = max(stof(ellipse->get("ry", "0")), 0.0f);
        // Negative or invalid radii means no visible ellipse

        float cx = stof(ellipse->get("cx", "0"));
        // Unspecified x-center means 0

        float cy = stof(ellipse->get("cy", "0"));
        // Unspecified y-center means 0

        Style style = _parse_style(ellipse->get_child("style"));
        const auto& xml_class = ellipse->get_child_optional("class");
        if (xml_class) {
            style.classes = _parse_class(*xml_class);
        }

        shared_ptr<EllipseShape> shape = make_shared<EllipseShape>(rx, ry);
        shape->setFillColor(style.fill);
        shape->setOutlineColor(style.stroke);
        shape->setOutlineThickness(style.stroke_width);
        shape->setPosition(cx - rx, cy - ry);

        return make_pair(shape, nullptr);
        // collision not yet supported
    }
}

ShapeManager::ShapePair ShapeManager::_parse_line(const ptree& xml) {
    // Assume that xml represents a <line> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "line");
#endif // DEBUG

    const auto& line = xml.get_child_optional("<xmlattr>");
    shared_ptr<Arrow> shape;
    shared_ptr<b2EdgeShape> b2shape;
    if (!line) {
        // If we see a blank <line/> element without attributes...
        shape = make_shared<Arrow>(Vector2f(), Vector2f(), Color::White, 0.0);
    }
    else {
        Vector2f v1(stof(line->get("x1", "0")), stof(line->get("y1", "0")));
        Vector2f v2(stof(line->get("x2", "0")), stof(line->get("y2", "0")));
        // Unspecified coordinates default to zero

        Style style = _parse_style(line->get_child("style"));
        const auto& xml_class = line->get_child_optional("class");
        if (xml_class) {
            style.classes = _parse_class(*xml_class);
        }

        shape = make_shared<Arrow>(v1, v2 - v1, style.stroke, style.stroke_width);

        const vector<string>& classes = style.classes;
        if (find(classes.begin(), classes.end(), COLLIDABLE_CLASS) != classes.end()) {
            // If this <line> is collidable...
            b2shape = make_shared<b2EdgeShape>();
            b2shape->Set(sfVecToB2Vec(v1), sfVecToB2Vec(v1-v2));
        }
    }

    //shape->setStyle(Arrow::Style::Line);

    return make_pair(shape, b2shape);
}

/// NOT SUPPORTED
ShapeManager::ShapePair ShapeManager::_parse_path(const ptree& xml) {
    return make_pair(nullptr, nullptr);
}

/// returns either a sf::ConvexShape if the given polygon is convex or a
/// thor::ConcaveShape if it's concave
ShapeManager::ShapePair ShapeManager::_parse_polygon(const ptree& xml) {
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

        return make_pair(shape, nullptr);
    }
    else {
        Style style = _parse_style(polygon->get_child("style"));
        if (style.fill == Color::Black) {
            // TODO: Remove this hack
            style.fill = Color::Transparent;
        }
        const auto& xml_class = polygon->get_child_optional("class");
        if (xml_class) {
            style.classes = _parse_class(*xml_class);
        }
        const auto& xml_transform = polygon->get_child_optional("transform");
        if (xml_transform) {
            style.transform = _parse_transform(*xml_transform);
        }

        vector<Vector2f> points = _parse_points(polygon->get_child("points"), style.transform);
        shared_ptr<b2Shape> b2shape;

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

            const vector<string>& classes = style.classes;
            if (find(classes.begin(), classes.end(), COLLIDABLE_CLASS) != classes.end()) {
                // If this polygon should be collidable...

                vector<b2Vec2> b2vecs = sfVecsToB2Vecs(points);
                if (find(classes.begin(), classes.end(), HOLLOW_CLASS) != classes.end()) {
                    // If this polygon should be hollow...
                    if (b2vecs.size() < 3) {
                        // If we have too many or too few vertices...
                        std::ostringstream err;
                        err << "Expected at least 3 vertices, got " << b2vecs.size();
                        throw std::invalid_argument(err.str());
                    }
                    b2shape = make_shared<b2ChainShape>();
                    static_pointer_cast<b2ChainShape>(b2shape)->CreateLoop(b2vecs.data(), b2vecs.size());
                }
                else {
                    if (b2vecs.size() < 3 || b2vecs.size() > b2_maxPolygonVertices) {
                        // If we have too many or too few vertices...
                        std::ostringstream err;
                        err << "Expected between 3 and " << b2_maxPolygonVertices
                            << " polygon vertices, got " << b2vecs.size();
                        throw std::invalid_argument(err.str());
                    }
                    b2shape = make_shared<b2PolygonShape>();
                    static_pointer_cast<b2PolygonShape>(b2shape)->Set(b2vecs.data(), b2vecs.size());
                }
            }

            return make_pair(shape, b2shape);
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

            const vector<string>& classes = style.classes;
            if (find(classes.begin(), classes.end(), COLLIDABLE_CLASS) != classes.end()) {
                // If this polygon should be collidable...

                vector<b2Vec2> b2vecs = sfVecsToB2Vecs(points);
                if (find(classes.begin(), classes.end(), HOLLOW_CLASS) != classes.end()) {
                    // If this polygon should be hollow...
                    if (b2vecs.size() < 3) {
                        // If we have too many or too few vertices...
                        std::ostringstream err;
                        err << "Expected at least 3 vertices, got " << b2vecs.size();
                        throw std::invalid_argument(err.str());
                    }
                    b2shape = make_shared<b2ChainShape>();
                    static_pointer_cast<b2ChainShape>(b2shape)->CreateLoop(b2vecs.data(), b2vecs.size());
                }
                else {
                    b2shape = make_shared<b2PolygonShape>();
                    static_pointer_cast<b2PolygonShape>(b2shape)->Set(b2vecs.data(), b2vecs.size());
                    if (b2vecs.size() < 3 || b2vecs.size() > b2_maxPolygonVertices) {
                        // If we have too many or too few vertices...
                        std::ostringstream err;
                        err << "Expected between 3 and " << b2_maxPolygonVertices
                            << " polygon vertices, got " << b2vecs.size();
                        throw std::invalid_argument(err.str());
                    }
                    else if (!static_pointer_cast<b2PolygonShape>(b2shape)->Validate()) {
                        std::ostringstream err;
                        err << "Solid polygons must be convex, but this one is concave";
                        throw std::invalid_argument(err.str());
                    }
                }
            }
            return make_pair(shape, b2shape);
        }
    }
}

ShapeManager::ShapePair ShapeManager::_parse_polyline(const ptree& xml) {
    // Assume that xml represents a <polyline> node
#ifdef DEBUG
    _throw_if_wrong_element(xml, "polyline");
#endif // DEBUG

    const auto& polyline = xml.get_child_optional("<xmlattr>");
    if (!polyline) {
        // If we see a blank <polyline/> element without attributes...
        shared_ptr<PolylineShape> shape = make_shared<PolylineShape>();
        shape->setFillColor(Color::Transparent);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0.0);

        return make_pair(shape, nullptr);
    }
    else {
        Style style = _parse_style(polyline->get_child("style"));
        const auto& xml_class = polyline->get_child_optional("class");
        if (xml_class) {
            style.classes = _parse_class(*xml_class);
        }
        if (style.fill == Color::Black) {
            // TODO: Remove this hack
            style.fill = Color::Transparent;
        }
        vector<Vector2f> points = _parse_points(polyline->get_child("points"));

        shared_ptr<PolylineShape> shape = make_shared<PolylineShape>(points);
        shared_ptr<b2ChainShape> b2shape;
        shape->setFillColor(style.fill);
        shape->setOutlineColor(style.stroke);
        shape->setOutlineThickness(style.stroke_width);

        const vector<string>& classes = style.classes;
        if (find(classes.begin(), classes.end(), COLLIDABLE_CLASS) != classes.end()) {
            // If this polyline should be collidable...
            b2shape = make_shared<b2ChainShape>();
            vector<b2Vec2> b2points = sfVecsToB2Vecs(points);
            b2shape->CreateChain(b2points.data(), b2points.size());
        }

        return make_pair(shape, b2shape);
    }
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

vector<Vector2f> ShapeManager::_parse_points(const ptree& xml, const Transform& xform) {
    vector<Vector2f> points;
    vector<string> points_str;
    boost::algorithm::split_regex(points_str, xml.data(), OPTIONAL_COMMA_REGEX);

    if (points_str.size() % 2 == 1) {
        // If we were given an odd number of coordinates...
        std::ostringstream err;
        err << "Expected an odd number of numbers, got " << points_str.size();
        throw std::invalid_argument(err.str());
    }

    points.reserve(points_str.size());
    for (int i = 0; i < points_str.size(); i += 2) {
        points.push_back(xform.transformPoint(stof(points_str[i]), stof(points_str[i+1])));
    }
    return points;
}

vector<string> ShapeManager::_parse_class(const ptree& xml) {
    smatch match;

    const string classtext = to_lower_copy(trim_copy(xml.data()));
    vector<string> classes;

    boost::algorithm::split_regex(classes, classtext, OPTIONAL_COMMA_REGEX);
    classes.shrink_to_fit();
    for (const string& i : classes) {
        regex_match(i, match, NAME_REGEX);
        if (match.size() <= 0) return vector<string>();
    }

    return classes;

}

Transform ShapeManager::_parse_transform(const ptree& xml) {
    vector<pair<string, vector<float>>> transforms;
    string data = xml.data();

    boost::sregex_iterator end;
    boost::sregex_iterator trans_it(data.begin(), data.end(), TRANSFORM_REGEX);
    for (; trans_it != end; ++trans_it) {
        // For each transformation declaration...

        string trans_str = (*trans_it)[0];
        string trans_name = (*trans_it)[1];
        std::cout << trans_str << std::endl;
        boost::sregex_iterator num_it(trans_str.begin(), trans_str.end(), SIGNED_DECIMAL_REGEX);
        vector<float> numbers;

        for (; num_it != end; ++num_it) {
            // And for each number within that transform's arguments...
            std::cout << num_it->str() << std::endl;
            numbers.emplace_back(stof(num_it->str()));
        }
        transforms.emplace_back(trans_name, numbers);
    }

    auto rend = transforms.rend();
    Transform xform;
    for (auto it = transforms.rbegin(); it != rend; ++it) {
        // For each transform from the last to the first...
        // (SVG specifies that translations are done from right to left)
        const vector<float>& nums = it->second;

        if (it->first == "translate") {
            xform.translate(it->second[0], (nums.size() >= 2) ? it->second[1] : 0.0);
        }
        else if (it->first == "scale") {
            xform.scale(it->second[0], (nums.size() >= 2) ? it->second[1] : 0.0);
        }
        else if (it->first == "rotate") {
            if (nums.size() >= 3) {
                xform.rotate(nums[0], nums[1], nums[2]);
            }
            else {
                xform.rotate(nums[0]);
            }
        }
        else if (it->first == "skewX") {
            Transform x2(1.0f, tan(toDegrees(nums[0])), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            xform.combine(x2);
        }
        else if (it->first == "skewY") {
            Transform x2(1.0f, 0.0f, 0.0f, tan(toDegrees(nums[0])), 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            xform.combine(x2);
        }
        else if (it->first == "matrix") {
            Transform x2(nums[0], nums[1], nums[2], nums[3], nums[4], nums[5], 0.0f, 0.0f, 0.0f);
            xform.combine(x2);
        }
    }

    return xform;
}

// TODO: Make this work
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

