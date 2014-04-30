#ifndef DEBUGDRAWING_HPP
#define DEBUGDRAWING_HPP

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <Thor/Graphics.hpp>
#include <Thor/Shapes.hpp>
#include <Thor/Vectors.hpp>

#include "util.hpp"

namespace util {
using sf::RenderWindow;
using sf::CircleShape;
using sf::Color;
using sf::Vector2f;
using sf::VertexArray;
using sf::PrimitiveType;
using sf::Vertex;
using sf::ConvexShape;
using thor::ConcaveShape;
using thor::Arrow;
class DebugDrawing : public b2Draw
{
    public:
        DebugDrawing();
        void setWindow(RenderWindow& window);
        virtual ~DebugDrawing();
        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) final override;
        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) final override;
        void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) final override;
        void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) final override;
        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) final override;
        void DrawTransform(const b2Transform& xf) final override;

        inline void _prep_circle(const b2Vec2& center, float32 radius, const b2Color& color);
        inline void _prep_color(const b2Color& color);
        inline void _prep_polygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    private:
        const static float LINE_THICKNESS;
        RenderWindow* _window;
        CircleShape _circle;
        ConvexShape _shape;
        Color _color;
        Arrow _arrow;

};
}

#endif // DEBUGDRAWING_HPP
