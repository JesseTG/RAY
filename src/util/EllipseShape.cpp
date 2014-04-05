#include "EllipseShape.hpp"

#include <cmath>

namespace sf {

const unsigned int EllipseShape::DEFAULT_POINTS = 30;

EllipseShape::EllipseShape(const float rx, const float ry, const unsigned int points) :
    _num_points(points),
    _radius_x(rx),
    _radius_y(ry) {
}

EllipseShape::~EllipseShape()
{
    //dtor
}

unsigned int EllipseShape::getPointCount() const {
    return this->_num_points;
}

Vector2f EllipseShape::getPoint(unsigned int index) const {
    static const float pi = 3.141592654f;

    float angle = index * 2 * pi / this->_num_points - pi / 2;
    float x = std::cos(angle) * this->_radius_x;
    float y = std::sin(angle) * this->_radius_y;

    return Vector2f(this->_radius_x + x, this->_radius_y + y);
}

}
