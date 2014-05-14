#include "entities.hpp"
#include "util.hpp"
#include "Group.hpp"

#include <exception>
#include <LuaContext.hpp>

#include <SFML/Audio.hpp>

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
using sf::Sound;
using sf::Music;
using sf::SoundSource;
using sf::Text;
using sf::Vector2;
using sf::Vector2i;
using sf::Vector2f;

template<class SFDrawableT>
void initCommonSFMLDrawableBindings(const string& name, LuaContext& lua) {

    lua.registerMember<SFDrawableT, Vector2f>("position",
    [](const SFDrawableT& s) {
        return s.getPosition();
    },
    [](SFDrawableT& s, const Vector2f& position) {
        s.setPosition(position);
    });
    lua.registerMember<SFDrawableT, float>("rotation",
    [](const SFDrawableT& s) {
        return s.getRotation();
    },
    [](SFDrawableT& s, const float rotation) {
        s.setRotation(rotation);
    });
    lua.registerMember<SFDrawableT, Vector2f>("origin",
    [](const SFDrawableT& s) {
        return s.getOrigin();
    },
    [](SFDrawableT& s, const Vector2f& origin) {
        s.setOrigin(origin);
    });
    lua.registerMember<SFDrawableT, Vector2f>("scale",
    [](const SFDrawableT& s) {
        return s.getScale();
    },
    [](SFDrawableT& s, const Vector2f& scale) {
        s.setScale(scale);
    });
    lua.registerFunction<void(SFDrawableT::*)(const Vector2f&)>("move", &SFDrawableT::move);
    lua.registerFunction<void(SFDrawableT::*)(const Vector2f&)>("scale", &SFDrawableT::scale);
    // TODO: Allow SFML vectors *and* floats
    lua.registerFunction("rotate", &SFDrawableT::rotate);
}

template<class SFShapeT>
void initCommonSFMLShapeBindings(const string& name, LuaContext& lua) {

    lua.registerMember<SFShapeT, Color>("fillColor",
    [](const SFShapeT& s) {
        return s.getFillColor();
    },
    [](SFShapeT& s, const Color& color) {
        s.setFillColor(color);
    });
    lua.registerMember<SFShapeT, Color>("outlineColor",
    [](const SFShapeT& s) {
        return s.getOutlineColor();
    },
    [](SFShapeT& s, const Color& color) {
        s.setOutlineColor(color);
    });
    lua.registerMember<SFShapeT, float>("outlineThickness",
    [](const SFShapeT& s) {
        return s.getOutlineThickness();
    },
    [](SFShapeT& s, const float thickness) {
        s.setOutlineThickness(thickness);
    });
}

template<class SFSoundSourceT>
void initCommonSFMLSoundSourceBindings(const string& name, LuaContext& lua) {
    lua.registerMember<SFSoundSourceT, bool>("loop",
    [](const SFSoundSourceT& sound) {
        return sound.getLoop();
    },
    [](SFSoundSourceT& sound, const bool loop) {
        sound.setLoop(loop);
    });

    lua.registerMember<SFSoundSourceT, bool>("relativeToListener",
    [](const SFSoundSourceT& sound) {
        return sound.isRelativeToListener();
    },
    [](SFSoundSourceT& sound, const bool relative) {
        sound.setRelativeToListener(relative);
    });

    lua.registerMember<SFSoundSourceT, float>("pitch",
    [](const SFSoundSourceT& sound) {
        return sound.getPitch();
    },
    [](SFSoundSourceT& sound, const float pitch) {
        sound.setPitch(pitch);
    });

    lua.registerMember<SFSoundSourceT, float>("attenuation",
    [](const SFSoundSourceT& sound) {
        return sound.getAttenuation();
    },
    [](SFSoundSourceT& sound, const float attenuation) {
        sound.setAttenuation(attenuation);
    });

    lua.registerMember<SFSoundSourceT, float>("minDistance",
    [](const SFSoundSourceT& sound) {
        return sound.getMinDistance();
    },
    [](SFSoundSourceT& sound, const float min_distance) {
        sound.setMinDistance(min_distance);
    });

    lua.registerMember<SFSoundSourceT, float>("volume",
    [](const SFSoundSourceT& sound) {
        return sound.getVolume();
    },
    [](SFSoundSourceT& sound, const float volume) {
        sound.setVolume(constrain(volume, 0.f, 100.f));
    });
}

void initSFMLTypeBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();

    lua.writeVariable("SFML", LuaEmptyArray);
    {
        lua.writeVariable("SFML", "Vector", LuaEmptyArray);
        {
            lua.writeFunction("SFML", "Vector", "new",
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
            lua.registerMember("x", &Vector2f::x);
            lua.registerMember("y", &Vector2f::y);
        }

        lua.writeVariable("SFML", "Color", LuaEmptyArray);
        {
            lua.writeFunction("SFML", "Color", "new", getDefaultConstructorLambda<Color>());
            lua.registerMember("r", &Color::r);
            lua.registerMember("g", &Color::g);
            lua.registerMember("b", &Color::b);
            lua.registerMember("a", &Color::a);

            lua.writeVariable("SFML", "Color", "Black", Color::Black);
            lua.writeVariable("SFML", "Color", "White", Color::White);
            lua.writeVariable("SFML", "Color", "Red", Color::Red);
            lua.writeVariable("SFML", "Color", "Green", Color::Green);
            lua.writeVariable("SFML", "Color", "Blue", Color::Blue);
            lua.writeVariable("SFML", "Color", "Yellow", Color::Yellow);
            lua.writeVariable("SFML", "Color", "Magenta", Color::Magenta);
            lua.writeVariable("SFML", "Color", "Cyan", Color::Cyan);
            lua.writeVariable("SFML", "Color", "Transparent", Color::Transparent);
        }

        lua.writeVariable("SFML", "Rect", LuaEmptyArray);
        {
            lua.writeFunction("SFML", "Rect", "new", getDefaultConstructorLambda<FloatRect>());
            lua.registerMember("left", &FloatRect::left);
            lua.registerMember("x", &FloatRect::left);
            lua.registerMember("top", &FloatRect::top);
            lua.registerMember("y", &FloatRect::top);
            lua.registerMember("width", &FloatRect::width);
            lua.registerMember("height", &FloatRect::height);
        }

        lua.writeVariable("SFML", "Sprite", LuaEmptyArray);
        {
            lua.writeFunction("SFML", "Sprite", "new", getDefaultConstructorLambda<Sprite>());
            initCommonSFMLDrawableBindings<Sprite>("Sprite", lua);
        }

        initCommonSFMLDrawableBindings<Transformable>("Transformable", lua);

        lua.writeVariable("SFML", "Drawable", LuaEmptyArray);
        {

            lua.registerMember<Drawable, Vector2f>("position",
            [](const Drawable& s) {
                return dynamic_cast<const Transformable*>(&s)->getPosition();
            },
            [](Drawable& s, const Vector2f& position) {
                dynamic_cast<Transformable*>(&s)->setPosition(position);
            });
            lua.registerMember<Drawable, float>("rotation",
            [](const Drawable& s) {
                return dynamic_cast<const Transformable*>(&s)->getRotation();
            },
            [](Drawable& s, const float rotation) {
                dynamic_cast<Transformable*>(&s)->setRotation(rotation);
            });
            lua.registerMember<Drawable, Vector2f>("origin",
            [](const Drawable& s) {
                return dynamic_cast<const Transformable*>(&s)->getOrigin();
            },
            [](Drawable& s, const Vector2f& origin) {
                dynamic_cast<Transformable*>(&s)->setOrigin(origin);
            });
            lua.registerMember<Drawable, Vector2f>("scale",
            [](const Drawable& s) {
                return dynamic_cast<const Transformable*>(&s)->getScale();
            },
            [](Drawable& s, const Vector2f& scale) {
                dynamic_cast<Transformable*>(&s)->setScale(scale);
            });
            //lua.registerFunction<void(Drawable::*)(const Vector2f&)>("move", &Drawable::move);
            //lua.registerFunction<void(Drawable::*)(const Vector2f&)>("scale", &Drawable::scale);
            // TODO: Allow SFML vectors *and* floats
            //lua.registerFunction("rotate", &Drawable::rotate);
        }

        lua.writeVariable("SFML", "CircleShape", LuaEmptyArray);
        {
            lua.writeFunction("SFML", "CircleShape", "new",
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
            initCommonSFMLDrawableBindings<CircleShape>("CircleShape", lua);
            initCommonSFMLShapeBindings<CircleShape>("CircleShape", lua);
            lua.registerMember<CircleShape, float>("radius",
            [](const CircleShape& circle) {
                return circle.getRadius();
            },
            [](CircleShape& circle, const float radius) {
                circle.setRadius(radius);
            });
        }

        lua.writeVariable("SFML", "Group", LuaEmptyArray);
        {
            initCommonSFMLDrawableBindings<Group>("Group", lua);
        }

        lua.writeVariable("SFML", "RectangleShape", LuaEmptyArray);
        {
            lua.writeFunction("SFML", "RectangleShape", "new",
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
            lua.registerMember<RectangleShape, Vector2f>("size",
            [](const RectangleShape& rect) {
                return rect.getSize();
            },
            [](RectangleShape& rect, const Vector2f& vec) {
                rect.setSize(vec);
            });
            initCommonSFMLDrawableBindings<RectangleShape>("RectangleShape", lua);
            initCommonSFMLShapeBindings<RectangleShape>("RectangleShape", lua);
        }

        lua.writeVariable("SFML", "Text", LuaEmptyArray);
        {
            lua.writeFunction("SFML", "Text", "new", []
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
            initCommonSFMLDrawableBindings<Text>("Text", lua);

            lua.registerMember<Text, string>("text",
            [](const Text& text) {
                return text.getString();
            },
            [](Text& text, const string& s) {
                text.setString(s);
            });
            lua.registerMember<Text, int>("characterSize",
            [](const Text& text) {
                return text.getCharacterSize();
            },
            [](Text& text, const int s) {
                text.setCharacterSize(s);
            });
            lua.registerMember<Text, Color>("color",
            [](const Text& text) {
                return text.getColor();
            },
            [](Text& text, const Color& c) {
                text.setColor(c);
            });
            lua.registerMember<Text, bool>("bold",
            [](const Text& text) {
                return text.getStyle() & Text::Style::Bold;
            },
            [](Text& text, const bool bold) {
                int style = (bold) ?
                            text.getStyle() | Text::Style::Bold :
                            text.getStyle() & ~Text::Style::Bold;
                text.setStyle(style);
            });
            lua.registerMember<Text, bool>("italic",
            [](const Text& text) {
                return text.getStyle() & Text::Style::Italic;
            },
            [](Text& text, const bool italic) {
                int style = (italic) ?
                            text.getStyle() | Text::Style::Italic :
                            text.getStyle() & ~Text::Style::Italic;
                text.setStyle(style);
            });
            lua.registerMember<Text, bool>("underlined",
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

        lua.writeVariable("SFML", "Audio", LuaEmptyArray);
        {
            lua.writeVariable("SFML", "Audio", "Status", LuaEmptyArray);
            {
                lua.writeVariable("SFML", "Audio", "Status", "Stopped", SoundSource::Stopped);
                lua.writeVariable("SFML", "Audio", "Status", "Paused", SoundSource::Paused);
                lua.writeVariable("SFML", "Audio", "Status", "Playing", SoundSource::Playing);
            }

            lua.writeVariable("SFML", "Audio", "Sound", LuaEmptyArray);
            {
                lua.writeFunction("SFML", "Audio", "Sound", "Get", [&game](const string& id) {
                    return game.getSoundManager()->getSound(id);
                });

                initCommonSFMLSoundSourceBindings<Sound>("Sound", lua);
                lua.registerFunction("play", &Sound::play);
                lua.registerFunction("pause", &Sound::pause);
                lua.registerFunction("stop", &Sound::stop);
            }

            lua.writeVariable("SFML", "Audio", "Music", LuaEmptyArray);
            {

                lua.writeFunction("SFML", "Audio", "Music", "Load", [&game](const string& id) {
                    game.getMusicManager()->setSong(id);
                    return &(game.getMusicManager()->getCurrentSong());
                });
                initCommonSFMLSoundSourceBindings<Music>("Music", lua);
                lua.registerFunction<Music, void(void)>("play", [](Music& music) {
                    music.play();
                });
                lua.registerFunction<Music, void(void)>("pause", [](Music& music) {
                    music.pause();
                });
                lua.registerFunction<Music, void(void)>("stop", [](Music& music) {
                    music.stop();
                });
            }
        }

    }
}
}
}

