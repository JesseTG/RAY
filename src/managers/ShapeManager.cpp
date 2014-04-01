#include "ShapeManager.hpp"

#include <algorithm>
#include <exception>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "util.hpp"

namespace ray {
using std::stof;
using std::max;

using boost::trim_copy;
using boost::to_lower_copy;
using boost::regex;
using boost::regex_match;
using boost::smatch;
unordered_map<string, const Color> ShapeManager::NAMED_COLORS = {

    { "red", Color(255, 0, 0) },
    { "green", Color( 0, 128, 0) },
    { "blue", Color( 0, 0, 255) },
    { "yellow", Color(255, 255, 0) },
    { "cyan", Color( 0, 255, 255) },
    { "magenta", Color(255, 0, 255) },
    { "black", Color( 0, 0, 0) },
    { "grey", Color(128, 128, 128) },
    { "gray", Color(128, 128, 128) },
    { "white", Color(255, 255, 255) },
    { "aliceblue", Color(240, 248, 255) },
    { "antiquewhite", Color(250, 235, 215) },
    { "aqua", Color( 0, 255, 255) },
    { "aquamarine", Color(127, 255, 212) },
    { "azure", Color(240, 255, 255) },
    { "beige", Color(245, 245, 220) },
    { "bisque", Color(255, 228, 196) },
    { "blanchedalmond", Color(255, 235, 205) },
    { "blueviolet", Color(138, 43, 226) },
    { "brown", Color(165, 42, 42) },
    { "burlywood", Color(222, 184, 135) },
    { "cadetblue", Color( 95, 158, 160) },
    { "chartreuse", Color(127, 255, 0) },
    { "chocolate", Color(210, 105, 30) },
    { "coral", Color(255, 127, 80) },
    { "cornflowerblue", Color(100, 149, 237) },
    { "cornsilk", Color(255, 248, 220) },
    { "crimson", Color(220, 20, 60) },
    { "darkblue", Color( 0, 0, 139) },
    { "darkcyan", Color( 0, 139, 139) },
    { "darkgoldenrod", Color(184, 134, 11) },
    { "darkgray", Color(169, 169, 169) },
    { "darkgreen", Color( 0, 100, 0) },
    { "darkgrey", Color(169, 169, 169) },
    { "darkkhaki", Color(189, 183, 107) },
    { "darkmagenta", Color(139, 0, 139) },
    { "darkolivegreen", Color( 85, 107, 47) },
    { "darkorange", Color(255, 140, 0) },
    { "darkorchid", Color(153, 50, 204) },
    { "darkred", Color(139, 0, 0) },
    { "darksalmon", Color(233, 150, 122) },
    { "darkseagreen", Color(143, 188, 143) },
    { "darkslateblue", Color( 72, 61, 139) },
    { "darkslategray", Color( 47, 79, 79) },
    { "darkslategrey", Color( 47, 79, 79) },
    { "darkturquoise", Color( 0, 206, 209) },
    { "darkviolet", Color(148, 0, 211) },
    { "deeppink", Color(255, 20, 147) },
    { "deepskyblue", Color( 0, 191, 255) },
    { "dimgray", Color(105, 105, 105) },
    { "dimgrey", Color(105, 105, 105) },
    { "dodgerblue", Color( 30, 144, 255) },
    { "firebrick", Color(178, 34, 34) },
    { "floralwhite", Color(255, 250, 240) },
    { "forestgreen", Color( 34, 139, 34) },
    { "fuchsia", Color(255, 0, 255) },
    { "gainsboro", Color(220, 220, 220) },
    { "ghostwhite", Color(248, 248, 255) },
    { "gold", Color(255, 215, 0) },
    { "goldenrod", Color(218, 165, 32) },
    { "greenyellow", Color(173, 255, 47) },
    { "honeydew", Color(240, 255, 240) },
    { "hotpink", Color(255, 105, 180) },
    { "indianred", Color(205, 92, 92) },
    { "indigo", Color( 75, 0, 130) },
    { "ivory", Color(255, 255, 240) },
    { "khaki", Color(240, 230, 140) },
    { "lavender", Color(230, 230, 250) },
    { "lavenderblush", Color(255, 240, 245) },
    { "lawngreen", Color(124, 252, 0) },
    { "lemonchiffon", Color(255, 250, 205) },
    { "lightblue", Color(173, 216, 230) },
    { "lightcoral", Color(240, 128, 128) },
    { "lightcyan", Color(224, 255, 255) },
    { "lightgoldenrodyellow", Color(250, 250, 210) },
    { "lightgray", Color(211, 211, 211) },
    { "lightgreen", Color(144, 238, 144) },
    { "lightgrey", Color(211, 211, 211) },
    { "lightpink", Color(255, 182, 193) },
    { "lightsalmon", Color(255, 160, 122) },
    { "lightseagreen", Color( 32, 178, 170) },
    { "lightskyblue", Color(135, 206, 250) },
    { "lightslategray", Color(119, 136, 153) },
    { "lightslategrey", Color(119, 136, 153) },
    { "lightsteelblue", Color(176, 196, 222) },
    { "lightyellow", Color(255, 255, 224) },
    { "lime", Color( 0, 255, 0) },
    { "limegreen", Color( 50, 205, 50) },
    { "linen", Color(250, 240, 230) },
    { "maroon", Color(128, 0, 0) },
    { "mediumaquamarine", Color(102, 205, 170) },
    { "mediumblue", Color( 0, 0, 205) },
    { "mediumorchid", Color(186, 85, 211) },
    { "mediumpurple", Color(147, 112, 219) },
    { "mediumseagreen", Color( 60, 179, 113) },
    { "mediumslateblue", Color(123, 104, 238) },
    { "mediumspringgreen", Color( 0, 250, 154) },
    { "mediumturquoise", Color( 72, 209, 204) },
    { "mediumvioletred", Color(199, 21, 133) },
    { "midnightblue", Color( 25, 25, 112) },
    { "mintcream", Color(245, 255, 250) },
    { "mistyrose", Color(255, 228, 225) },
    { "moccasin", Color(255, 228, 181) },
    { "navajowhite", Color(255, 222, 173) },
    { "navy", Color( 0, 0, 128) },
    { "oldlace", Color(253, 245, 230) },
    { "olive", Color(128, 128, 0) },
    { "olivedrab", Color(107, 142, 35) },
    { "orange", Color(255, 165, 0) },
    { "orangered", Color(255, 69, 0) },
    { "orchid", Color(218, 112, 214) },
    { "palegoldenrod", Color(238, 232, 170) },
    { "palegreen", Color(152, 251, 152) },
    { "paleturquoise", Color(175, 238, 238) },
    { "palevioletred", Color(219, 112, 147) },
    { "papayawhip", Color(255, 239, 213) },
    { "peachpuff", Color(255, 218, 185) },
    { "peru", Color(205, 133, 63) },
    { "pink", Color(255, 192, 203) },
    { "plum", Color(221, 160, 221) },
    { "powderblue", Color(176, 224, 230) },
    { "purple", Color(128, 0, 128) },
    { "rosybrown", Color(188, 143, 143) },
    { "royalblue", Color( 65, 105, 225) },
    { "saddlebrown", Color(139, 69, 19) },
    { "salmon", Color(250, 128, 114) },
    { "sandybrown", Color(244, 164, 96) },
    { "seagreen", Color( 46, 139, 87) },
    { "seashell", Color(255, 245, 238) },
    { "sienna", Color(160, 82, 45) },
    { "silver", Color(192, 192, 192) },
    { "skyblue", Color(135, 206, 235) },
    { "slateblue", Color(106, 90, 205) },
    { "slategray", Color(112, 128, 144) },
    { "slategrey", Color(112, 128, 144) },
    { "snow", Color(255, 250, 250) },
    { "springgreen", Color( 0, 255, 127) },
    { "steelblue", Color( 70, 130, 180) },
    { "tan", Color(210, 180, 140) },
    { "teal", Color( 0, 128, 128) },
    { "thistle", Color(216, 191, 216) },
    { "tomato", Color(255, 99, 71) },
    { "transparent", Color::Transparent },
    // ^ Not in the SVG standard, but convenient for our purposes
    { "turquoise", Color( 64, 224, 208) },
    { "violet", Color(238, 130, 238) },
    { "wheat", Color(245, 222, 179) },
    { "whitesmoke", Color(245, 245, 245) },
    { "yellowgreen", Color(154, 205, 50) },
};

const regex ShapeManager::HEX_COLOR("#([0-9a-fA-f]{3})([0-9a-fA-f]{3})?");
const regex ShapeManager::RGB_COLOR(
    "rgb\\s*\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)"
);
const regex ShapeManager::RGB_PERCENT_COLOR(
    "rgb\\s*\\(\\s*(\\d{1,3})\\s*%\\s*,\\s*(\\d{1,3})\\s*%\\s*,\\s*(\\d{1,3})\\s*%\\s*\\)"
);

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
        this->_parse_shape(xml);
    }

    return false;
}

void ShapeManager::_parse_shape(const ptree& xml) {

}

shared_ptr<Shape> ShapeManager::_parse_circle(const ptree& xml) {
    // Assume that xml represents a <circle> node
#ifdef DEBUG
    if (xml.data() != "circle") {
        std::ostringstream err;
        err << "Expected a <circle> element, got " << xml.data();
        throw std::invalid_argument(err.str());
    }
#endif // DEBUG

    const auto& circle = xml.get_child_optional("circle.<xmlattr>");
    if (!circle) {
        // If we see a blank <circle/> element without attributes...
        return make_shared<CircleShape>();
        // TODO: Doesn't imply the same things the below definitions do
    }
    else {
        float r = max(stof(circle->get("r", "0")), 0.0f);
        // Negative or invalid radii means no visible circle

        float cx = stof(circle->get("cx", "0"));
        // Unspecified x-center means 0

        float cy = stof(circle->get("cy", "0"));
        // Unspecified y-center means 0

        Color fill = _parse_color(circle->get("fill", "black"));
        // Unspecified color means black

        float fill_opacity = constrain(stof(circle->get("fill-opacity", "1.0")), 0.0f, 1.0f);
        // Unspecified fill opacity means fully opaque

        Color stroke = _parse_color(circle->get("stroke", "transparent"));
        // Unspecified stroke color means no stroke

        float stroke_width = stof(circle->get("stroke-width", "1.0"));
        // Unspecified stroke width is one unit

        float stroke_opacity = constrain(stof(circle->get("stroke-opacity", "1.0")), 0.0f, 1.0f);
        // Unspecified stroke opacity means fully opaque

        fill.a = fill_opacity * 255; // Map 0-1 to 0-255
        stroke.a = stroke_opacity * 255; // Here, too

        shared_ptr<CircleShape> shape = make_shared<CircleShape>(r);
        shape->setFillColor(fill);
        shape->setOutlineColor(stroke);
        shape->setOutlineThickness(stroke_width);
        shape->setPosition(cx - r, cy - r);

        return shape;
    }
}

shared_ptr<Shape> ShapeManager::_parse_ellipse(const ptree& xml) {
    return nullptr;
}

shared_ptr<Shape> ShapeManager::_parse_line(const ptree& xml) {
    return nullptr;
}

shared_ptr<Shape> ShapeManager::_parse_path(const ptree& xml) {
    return nullptr;
}

shared_ptr<Shape> ShapeManager::_parse_polygon(const ptree& xml) {
    return nullptr;
}

shared_ptr<Shape> ShapeManager::_parse_polyline(const ptree& xml) {
    return nullptr;
}

shared_ptr<Shape> ShapeManager::_parse_rect(const ptree& xml) {
    return nullptr;
}

const Color ShapeManager::_parse_color(const string& text) {
    const string colortext = to_lower_copy(trim_copy(text));
    if (NAMED_COLORS.count(colortext)) {
        // If this is a predefined color...
        return NAMED_COLORS[colortext];
    }
    else {
//    smatch match1 = regex_match(HEX_COLOR);
  ///      smatch match2 = regex_match(RGB_COLOR);
     //   smatch match3 = regex_match(RGB_PERCENT_COLOR);
        return Color(255, 255, 255); // TODO: IMPLEMENT
    }
}

}
