#include "entities.hpp"
#include "util.hpp"

#include <iostream>
#include <exception>
#include <LuaContext.hpp>

namespace ray {
namespace entities {
using std::invalid_argument;
using std::logic_error;
using boost::get;
using boost::optional;
using boost::variant;

void initBox2DTypeBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();

    lua.writeVariable("Box2D", LuaEmptyArray);
    {
        lua.writeVariable("Box2D", "Vector", LuaEmptyArray);
        {
            lua.writeFunction("Box2D", "Vector", "new",
            [](const optional<float>& arg1, const optional<float>& arg2) {
                if (arg1 && arg2) {
                    // If the scripter passed in two arguments...
                    return b2Vec2(*arg1, *arg2);
                }
                else if (!(arg1 || arg2)) {
                    // Else if the scripter passed in *no* arguments...
                    return b2Vec2(0, 0);
                }
                else {
                    throw invalid_argument(
                        "Must pass in two numbers or nothing at all"
                    );
                }

            });
            lua.registerMember("x", &b2Vec2::x);
            lua.registerMember("y", &b2Vec2::y);
        }

        lua.writeVariable("Box2D", "BodyType", LuaEmptyArray);
        {
            lua.writeVariable("Box2D", "BodyType", "Static", b2_staticBody);
            lua.writeVariable("Box2D", "BodyType", "Dynamic", b2_dynamicBody);
            lua.writeVariable("Box2D", "BodyType", "Kinematic", b2_kinematicBody);
        }

        lua.writeVariable("Box2D", "BodyDef", LuaEmptyArray);
        {
            lua.writeFunction("Box2D", "BodyDef", "new", getDefaultConstructorLambda<b2BodyDef>());

            lua.registerMember("active", &b2BodyDef::active);
            lua.registerMember("allowSleep", &b2BodyDef::allowSleep);
            lua.registerMember("angle", &b2BodyDef::angle);
            lua.registerMember("angularDamping", &b2BodyDef::angularDamping);
            lua.registerMember("angularVelocity", &b2BodyDef::angularVelocity);
            lua.registerMember("awake", &b2BodyDef::awake);
            lua.registerMember("bullet", &b2BodyDef::bullet);
            lua.registerMember("fixedRotation", &b2BodyDef::fixedRotation);
            lua.registerMember("gravityScale", &b2BodyDef::gravityScale);
            lua.registerMember("linearDamping", &b2BodyDef::linearDamping);
            lua.registerMember("linearVelocity", &b2BodyDef::linearVelocity);
            lua.registerMember("position", &b2BodyDef::position);
            lua.registerMember("type", &b2BodyDef::type);
        }

        lua.writeVariable("Box2D", "Body", LuaEmptyArray);
        {
            lua.writeFunction("Box2D", "Body", "new", [&game](const b2BodyDef& def) {
                return game.getPhysicsWorld()->CreateBody(&def);
            });
            lua.registerMember<b2Body, bool>("awake",
            [](const b2Body& object) {
                return object.IsAwake();
            },
            [](b2Body& object, const bool val) {
                object.SetAwake(val);
            });
            lua.registerMember<b2Body, bool>("fixedRotation",
            [](const b2Body& object) {
                return object.IsFixedRotation();
            },
            [](b2Body& object, const bool val) {
                object.SetFixedRotation(val);
            });
            lua.registerMember<b2Body, b2Vec2>("position",
            [](const b2Body& body) {
                return body.GetPosition();
            },
            [](b2Body& body, const b2Vec2& pos) {
                body.SetTransform(pos, body.GetAngle());
            });
            lua.registerFunction<b2Body, b2Fixture*(b2FixtureDef&)>("CreateFixture",
            [](b2Body& body, b2FixtureDef& def) {
                return body.CreateFixture(&def);
            });
        }

        lua.writeVariable("Box2D", "Filter", LuaEmptyArray);
        {
            lua.writeFunction("Box2D", "Filter", "new", getDefaultConstructorLambda<b2Filter>());
            lua.registerMember("categoryBits", &b2Filter::categoryBits);
            lua.registerMember("maskBits", &b2Filter::maskBits);
            lua.registerMember("groupIndex", &b2Filter::groupIndex);
        }

        lua.writeVariable("Box2D", "FixtureDef", LuaEmptyArray);
        {
            lua.writeFunction("Box2D", "FixtureDef", "new", getDefaultConstructorLambda<b2FixtureDef>());
            lua.registerMember("friction", &b2FixtureDef::friction);
            lua.registerMember("restitution", &b2FixtureDef::restitution);
            lua.registerMember("density", &b2FixtureDef::density);
            lua.registerMember("isSensor", &b2FixtureDef::isSensor);
            lua.registerMember("filter", &b2FixtureDef::filter);
            typedef variant<b2CircleShape*, b2EdgeShape*, b2PolygonShape*, b2ChainShape*> shapevariant;
            lua.registerFunction<b2FixtureDef, void(shapevariant)>("setShape",
                    // TODO: Turn this into a member
            [](b2FixtureDef& def, shapevariant shape) {
                // NOTE: This depends on the order in which the members of the
                // b2Shape::Type enum are defined!
                switch (shape.which()) {
                    case b2Shape::Type::e_circle:
                        def.shape = *get<b2CircleShape*>(&shape);
                        break;
                    case b2Shape::Type::e_edge:
                        def.shape = *get<b2EdgeShape*>(&shape);
                        break;
                    case b2Shape::Type::e_polygon:
                        def.shape = *get<b2PolygonShape*>(&shape);
                        break;
                    case b2Shape::Type::e_chain:
                        def.shape = *get<b2ChainShape*>(&shape);
                        break;
                    default:
                        throw logic_error(
                            "Unknown b2Shape type passed in to Box2D.FixtureDef"
                        );
                }
            });
            lua.registerFunction<b2FixtureDef, const b2Shape*(void)>("getShape",
            [](b2FixtureDef& def) {
                return def.shape;
            });
        }

        lua.writeVariable("Box2D", "Fixture", LuaEmptyArray);
        {
            lua.registerMember<b2Fixture, float>("friction",
            [](const b2Fixture& object) {
                return object.GetFriction();
            },
            [](b2Fixture& object, const float val) {
                object.SetFriction(val);
            });
            lua.registerMember<b2Fixture, float>("restitution",
            [](const b2Fixture& object) {
                return object.GetRestitution();
            },
            [](b2Fixture& object, const float val) {
                object.SetRestitution(val);
            });
            lua.registerMember<b2Fixture, float>("density",
            [](const b2Fixture& object) {
                return object.GetDensity();
            },
            [](b2Fixture& object, const float val) {
                object.SetDensity(val);
            });
            lua.registerMember<b2Fixture, bool>("isSensor",
            [](const b2Fixture& object) {
                return object.IsSensor();
            },
            [](b2Fixture& object, const bool val) {
                object.SetSensor(val);
            });
        }

        lua.writeVariable("Box2D", "Shape", LuaEmptyArray);
        {
            lua.writeVariable("Box2D", "Shape", "Type", LuaEmptyArray);
            {
                lua.writeVariable("Box2D", "Shape", "Type", "Circle", b2Shape::e_circle);
                lua.writeVariable("Box2D", "Shape", "Type", "Edge", b2Shape::e_edge);
                lua.writeVariable("Box2D", "Shape", "Type", "Polygon", b2Shape::e_polygon);
                lua.writeVariable("Box2D", "Shape", "Type", "Chain", b2Shape::e_chain);
                lua.writeVariable("Box2D", "Shape", "Type", "Count", b2Shape::e_typeCount);
            }

            lua.writeVariable("Box2D", "Shape", "Circle", LuaEmptyArray);
            {
                lua.writeFunction("Box2D", "Shape", "Circle", "new",
                [](optional<float> r) {
                    b2CircleShape* shape = new b2CircleShape;
                    if (r) shape->m_radius = *r;

                    return shape;
                });
                lua.registerMember("position", &b2CircleShape::m_p);

                lua.registerMember<b2CircleShape, float>("radius",
                [](const b2CircleShape& shape) {
                    return shape.m_radius;
                },
                [](b2CircleShape& shape, const float rad) {
                    shape.m_radius = rad;
                });
                lua.registerMember<b2CircleShape, b2CircleShape::Type, b2CircleShape::Type(const b2CircleShape&)>("type",
                [](const b2CircleShape& shape) {
                    return shape.m_type;
                });
            }

            lua.writeVariable("Box2D", "Shape", "Edge", LuaEmptyArray);
            {
                lua.writeFunction("Box2D", "Shape", "Edge", "new", getNewDefaultConstructorLambda<b2EdgeShape>());
                lua.registerMember("v1", &b2EdgeShape::m_vertex1);
                lua.registerMember("v2", &b2EdgeShape::m_vertex2);
                // ^ The two vertices actually in the edge

                lua.registerMember("v0", &b2EdgeShape::m_vertex0);
                lua.registerMember("v3", &b2EdgeShape::m_vertex3);
                lua.registerMember("hasv0", &b2EdgeShape::m_hasVertex0);
                lua.registerMember("hasv3", &b2EdgeShape::m_hasVertex3);
                // ^ Two "ghost vertices"; not sure what that means, gotta look
                // more into it

                lua.registerMember<b2EdgeShape, b2EdgeShape::Type, b2EdgeShape::Type(const b2EdgeShape&)>("type",
                [](const b2EdgeShape& shape) {
                    return shape.m_type;
                });
            }

            lua.writeVariable("Box2D", "Shape", "Polygon", LuaEmptyArray);
            {
                lua.writeFunction("Box2D", "Shape", "Polygon", "new", getNewDefaultConstructorLambda<b2PolygonShape>());
            }

            lua.writeVariable("Box2D", "Shape", "Rectangle", LuaEmptyArray);
            {
                lua.writeFunction("Box2D", "Shape", "Rectangle", "new",
                [](const float hx, const float hy, const optional<b2Vec2&> center, const optional<float> angle) {
                    b2PolygonShape* box = new b2PolygonShape;
                    if (center && angle) {
                        box->SetAsBox(hx, hy, *center, *angle);
                    }
                    else {
                        box->SetAsBox(hx, hy);
                    }

                    return box;
                });
            }
        }
    }
}
}
}

