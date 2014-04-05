#ifndef ELLIPSESHAPE_HPP
#define ELLIPSESHAPE_HPP

#include <SFML/Graphics.hpp>


namespace sf {

using sf::Vector2f;
using sf::Shape;

class EllipseShape : public sf::Shape
{
    public:
        EllipseShape(const float rx=0.0, const float ry=0.0, const unsigned int points=DEFAULT_POINTS);
        virtual ~EllipseShape();
        unsigned int getPointCount() const override;
        Vector2f getPoint(unsigned int index) const override;
    private:
        unsigned int _num_points;
        float _radius_x;
        float _radius_y;
        const static unsigned int DEFAULT_POINTS;

};
}

#endif // ELLIPSESHAPE_HPP
