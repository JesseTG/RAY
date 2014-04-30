#include "DebugDrawing.hpp"

#include "config.hpp"

namespace util {
const float DebugDrawing::LINE_THICKNESS = 2.f;
using ray::PIXELS_PER_METER;
using ray::METERS_PER_PIXEL;
DebugDrawing::DebugDrawing() {
    this->SetFlags(b2Draw::e_shapeBit);
    this->_color.a = 128;
    this->_circle.setOutlineThickness(LINE_THICKNESS);
    this->_arrow.setThickness(LINE_THICKNESS);
    this->_shape.setOutlineThickness(LINE_THICKNESS);
}

DebugDrawing::~DebugDrawing()
{
    //dtor
}

void DebugDrawing::setWindow(RenderWindow& window) {
    this->_window = &window;
}

void DebugDrawing::_prep_color(const b2Color& color) {
    this->_color.r = color.r * 255;
    this->_color.g = color.g * 255;
    this->_color.b = color.b * 255;
}

void DebugDrawing::_prep_polygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    this->_prep_color(color);
    this->_shape.setPointCount(vertexCount);
    Vector2f v;
    for (int i = 0; i < vertexCount; ++i) {
        v.x = PIXELS_PER_METER * vertices[i].x;
        v.y = PIXELS_PER_METER * vertices[i].y;
        this->_shape.setPoint(i, v);
    }
}

void DebugDrawing::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    this->_prep_polygon(vertices, vertexCount, color);
    this->_shape.setOutlineColor(this->_color);
    this->_shape.setFillColor(Color::Transparent);
    this->_window->draw(this->_shape);
}

void DebugDrawing::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    this->_prep_polygon(vertices, vertexCount, color);
    this->_shape.setOutlineColor(Color::Transparent);
    this->_shape.setFillColor(this->_color);
    this->_window->draw(this->_shape);
}

void DebugDrawing::_prep_circle(const b2Vec2& center, float32 radius, const b2Color& color) {
    float r = PIXELS_PER_METER * radius;
    this->_circle.setRadius(r);
    this->_circle.setOrigin(r, r);
    this->_circle.setPosition(PIXELS_PER_METER * center.x, PIXELS_PER_METER * center.y);
    this->_prep_color(color);
}

void DebugDrawing::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
    this->_prep_circle(center, radius, color);
    this->_circle.setFillColor(Color::Transparent);
    this->_circle.setOutlineColor(this->_color);
    this->_window->draw(this->_circle);
}

void DebugDrawing::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
    this->_prep_circle(center, radius, color);
    this->_circle.setOutlineColor(Color::Transparent);
    this->_circle.setFillColor(this->_color);

    float r = PIXELS_PER_METER * radius;
    this->_arrow.setStyle(Arrow::Style::Line);
    this->_arrow.setPosition(PIXELS_PER_METER * center.x, PIXELS_PER_METER * center.y);
    this->_arrow.setDirection(axis.x * r, axis.y * r);
    this->_arrow.setColor(this->_color);

    this->_window->draw(this->_circle);
    this->_window->draw(this->_arrow);
}

void DebugDrawing::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    this->_prep_color(color);
    this->_arrow.setStyle(Arrow::Style::Line);
    this->_arrow.setPosition(PIXELS_PER_METER * p1.x, PIXELS_PER_METER * p1.y);
    this->_arrow.setDirection(PIXELS_PER_METER * (p2.x - p1.x), PIXELS_PER_METER * (p2.y - p1.y));
    this->_arrow.setColor(this->_color);

    this->_window->draw(this->_arrow);
}

void DebugDrawing::DrawTransform(const b2Transform& xf) {

}

}
