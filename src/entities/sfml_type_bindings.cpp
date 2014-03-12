#include "entities.hpp"
#include "util.hpp"

#include <LuaContext.hpp>

namespace ray {
namespace entities {
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

template<class SFDrawableT>
void initCommonSFMLDrawableBindings(const string& name) {

    _lua->registerMember<SFDrawableT, Vector2f>("position",
    [](const SFDrawableT& s) {
        return s.getPosition();
    },
    [](SFDrawableT& s, const Vector2f& position) {
        s.setPosition(position);
    });
    _lua->registerMember<SFDrawableT, float>("rotation",
    [](const SFDrawableT& s) {
        return s.getRotation();
    },
    [](SFDrawableT& s, const float rotation) {
        s.setRotation(rotation);
    });
    _lua->registerMember<SFDrawableT, Vector2f>("origin",
    [](const SFDrawableT& s) {
        return s.getOrigin();
    },
    [](SFDrawableT& s, const Vector2f& origin) {
        s.setOrigin(origin);
    });
    _lua->registerMember<SFDrawableT, Vector2f>("scale",
    [](const SFDrawableT& s) {
        return s.getScale();
    },
    [](SFDrawableT& s, const Vector2f& scale) {
        s.setScale(scale);
    });
    _lua->registerFunction<void(SFDrawableT::*)(const Vector2f&)>("move", &SFDrawableT::move);
    _lua->registerFunction<void(SFDrawableT::*)(const Vector2f&)>("scale", &SFDrawableT::scale);
    _lua->registerFunction("rotate", &SFDrawableT::rotate);
}

template<class SFShapeT>
void initCommonSFMLShapeBindings(const string& name) {

    _lua->registerMember<SFShapeT, Color>("fillColor",
    [](const SFShapeT& s) {
        return s.getFillColor();
    },
    [](SFShapeT& s, const Color& color) {
        s.setFillColor(color);
    });
    _lua->registerMember<SFShapeT, Color>("outLineColor",
    [](const SFShapeT& s) {
        return s.getOutlineColor();
    },
    [](SFShapeT& s, const Color& color) {
        s.setOutlineColor(color);
    });
    _lua->registerMember<SFShapeT, float>("outlineThickness",
    [](const SFShapeT& s) {
        return s.getOutlineThickness();
    },
    [](SFShapeT& s, const float thickness) {
        s.setOutlineThickness(thickness);
    });
}

void initSFMLTypeBindings() {
    _lua->writeVariable("SFML", LuaEmptyArray);
    {
        _lua->writeVariable("SFML", "Vector", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Vector", "new", getDefaultConstructorLambda<Vector2f>());
            _lua->registerMember("x", &Vector2f::x);
            _lua->registerMember("y", &Vector2f::y);
        }

        _lua->writeVariable("SFML", "Color", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Color", "new", getDefaultConstructorLambda<Color>());
            _lua->registerMember("r", &Color::r);
            _lua->registerMember("g", &Color::g);
            _lua->registerMember("b", &Color::b);
            _lua->registerMember("a", &Color::a);

            _lua->writeVariable("SFML", "Color", "Black", Color::Black);
            _lua->writeVariable("SFML", "Color", "White", Color::White);
            _lua->writeVariable("SFML", "Color", "Red", Color::Red);
            _lua->writeVariable("SFML", "Color", "Green", Color::Green);
            _lua->writeVariable("SFML", "Color", "Blue", Color::Blue);
            _lua->writeVariable("SFML", "Color", "Yellow", Color::Yellow);
            _lua->writeVariable("SFML", "Color", "Magenta", Color::Magenta);
            _lua->writeVariable("SFML", "Color", "Cyan", Color::Cyan);
            _lua->writeVariable("SFML", "Color", "Transparent", Color::Transparent);
        }

        _lua->writeVariable("SFML", "Rect", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Rect", "new", getDefaultConstructorLambda<FloatRect>());
            _lua->registerMember("left", &FloatRect::left);
            _lua->registerMember("x", &FloatRect::left);
            _lua->registerMember("top", &FloatRect::top);
            _lua->registerMember("y", &FloatRect::top);
            _lua->registerMember("width", &FloatRect::width);
            _lua->registerMember("height", &FloatRect::height);
        }

        _lua->writeVariable("SFML", "Sprite", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Sprite", "new", getDefaultConstructorLambda<Sprite>());
            initCommonSFMLDrawableBindings<Sprite>("Sprite");
        }

        _lua->writeVariable("SFML", "CircleShape", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "CircleShape", "new", [](const optional<float> radius) {
                return new CircleShape((radius) ? *radius : 0.0);
            });
            initCommonSFMLDrawableBindings<CircleShape>("CircleShape");
            initCommonSFMLShapeBindings<CircleShape>("CircleShape");
            _lua->registerMember<CircleShape, float>("radius",
            [](const CircleShape& circle) {
                return circle.getRadius();
            },
            [](CircleShape& circle, const float radius) {
                circle.setRadius(radius);
            });
        }
    }
}
}
}

