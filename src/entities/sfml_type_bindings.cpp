#include "entities.hpp"
#include "util.hpp"

#include <exception>
#include <LuaContext.hpp>

namespace ray {
namespace entities {
using std::invalid_argument;
using boost::get;
using boost::optional;
using boost::variant;
using sf::CircleShape;
using sf::Color;
using sf::ConvexShape;
using sf::Font;
using sf::Rect;
using sf::IntRect;
using sf::FloatRect;
using sf::RectangleShape;
using sf::Sprite;
using sf::Text;
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
    // TODO: Allow SFML vectors *and* floats
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
    _lua->registerMember<SFShapeT, Color>("outlineColor",
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
            _lua->writeFunction("SFML", "Vector", "new",
            [](const optional<float>& arg1, const optional<float>& arg2) {
                if (arg1 && arg2) {
                    // If the scripter passed in two arguments...
                    return Vector2f(*arg1, *arg2);
                }
                else if (!(arg1 || arg2)) {
                    // Else if the scripter passed in *no* arguments...
                    return Vector2f(0, 0);
                }
                else {
                    throw invalid_argument(
                        "Must pass in two numbers or nothing at all"
                    );
                }
            });
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
            _lua->writeFunction("SFML", "CircleShape", "new",
            [](const optional<float> radius, const optional<int> points) {
                if (radius && points) {
                    // If the user specified both the radius of the circle
                    // and the smoothness of it...
                    return new CircleShape(*radius, *points);
                }
                else if (radius && !points) {
                    // Else if the user just gave the radius...
                    return new CircleShape(*radius);
                }
                else {
                    return new CircleShape;
                }
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

        _lua->writeVariable("SFML", "RectangleShape", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "RectangleShape", "new",
            [](const optional<float>& arg1, const optional<float>& arg2) {
                if (arg1 && arg2) {
                    // If the scripter passed in two arguments...
                    return new RectangleShape(Vector2f(*arg1, *arg2));
                }
                else if (!(arg1 || arg2)) {
                    // Else if the scripter passed in *no* arguments...
                    return new RectangleShape;
                }
                else {
                    throw invalid_argument(
                        "Must pass in two numbers or nothing at all"
                    );
                }
            });
            _lua->registerMember<RectangleShape, Vector2f>("size",
            [](const RectangleShape& rect) {
                return rect.getSize();
            },
            [](RectangleShape& rect, const Vector2f& vec) {
                rect.setSize(vec);
            });
            initCommonSFMLDrawableBindings<RectangleShape>("RectangleShape");
            initCommonSFMLShapeBindings<RectangleShape>("RectangleShape");
        }

        _lua->writeVariable("SFML", "Text", LuaEmptyArray);
        {
            _lua->writeFunction("SFML", "Text", "new", []
                                (
                                    const optional<string>& arg1,
                                    const optional<Font&>& arg2,
                                    const optional<int>& arg3
            ) {
                if (arg1 && arg2 && arg3) {
                    // If the user specified text, a font, and a size...
                    return new Text(*arg1, *arg2, *arg3);
                }
                else if (arg1 && arg2) {
                    // Else if the user just specified a text and a font...
                    return new Text(*arg1, *arg2);
                }
                else if (arg1) {
                    // Else if the user just specified text...
                    Text* t = new Text;
                    t->setString(*arg1);
                    return t;
                }
                else {
                    return new Text;
                }
            });
            initCommonSFMLDrawableBindings<Text>("Text");

            _lua->registerMember<Text, string>("text",
            [](const Text& text) {
                return text.getString();
            },
            [](Text& text, const string& s) {
                text.setString(s);
            });
            _lua->registerMember<Text, int>("characterSize",
            [](const Text& text) {
                return text.getCharacterSize();
            },
            [](Text& text, const int s) {
                text.setCharacterSize(s);
            });
            _lua->registerMember<Text, Color>("color",
            [](const Text& text) {
                return text.getColor();
            },
            [](Text& text, const Color& c) {
                text.setColor(c);
            });
            _lua->registerMember<Text, bool>("bold",
            [](const Text& text) {
                return text.getStyle() & Text::Style::Bold;
            },
            [](Text& text, const bool bold) {
                int style = (bold) ?
                            text.getStyle() | Text::Style::Bold :
                            text.getStyle() & ~Text::Style::Bold;
                text.setStyle(style);
            });
            _lua->registerMember<Text, bool>("italic",
            [](const Text& text) {
                return text.getStyle() & Text::Style::Italic;
            },
            [](Text& text, const bool italic) {
                int style = (italic) ?
                            text.getStyle() | Text::Style::Italic :
                            text.getStyle() & ~Text::Style::Italic;
                text.setStyle(style);
            });
            _lua->registerMember<Text, bool>("underlined",
            [](const Text& text) {
                return text.getStyle() & Text::Style::Underlined;
            },
            [](Text& text, const bool underlined) {
                int style = (underlined) ?
                            text.getStyle() | Text::Style::Underlined:
                            text.getStyle() & ~Text::Style::Underlined;
                text.setStyle(style);
            });

        }

    }
}
}
}

