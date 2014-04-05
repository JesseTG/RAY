#ifndef POLYLINESHAPE_HPP
#define POLYLINESHAPE_HPP

#include <vector>

#include <SFML/Graphics.hpp>

namespace sf {
using std::vector;
using sf::Drawable;
using sf::Transformable;
using sf::RenderTarget;
using sf::RenderStates;
using sf::Shape;
using sf::Color;
using sf::Vector2f;
using sf::Texture;
using sf::VertexArray;
using sf::FloatRect;

class PolylineShape : public Drawable, public Transformable
{
    public:
        PolylineShape();

        template<class Container>
        PolylineShape(const Container& points) :
            _points(points.begin(), points.end())
        {
            this->_fill_vertices.setPrimitiveType(FILL_PRIMITIVE_TYPE);
            this->_outline_vertices.setPrimitiveType(OUTLINE_PRIMITIVE_TYPE);
        }

        virtual ~PolylineShape();
        unsigned int getPointCount() const;
        Vector2f getPoint(const unsigned int index) const;
        void setPoint(const int index, const Vector2f& pos);
        void setPoint(const int index, const float x, const float y);
        void setNumPoints(const int num);
        void setFillColor(const Color& color);
        void setFillColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        void setOutlineColor(const Color& color);
        void setOutlineColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        void setOutlineThickness(const float thickness);
    protected:
        void draw(RenderTarget& target, RenderStates states) const override;
        void update();
    private:
        mutable VertexArray _fill_vertices;
        mutable VertexArray _outline_vertices;
        Color _fill_color;
        Color _outline_color;
        float _outline_thickness;
        Texture* _texture;
        vector<Vector2f> _points;
        FloatRect _inside_bounds;
        FloatRect _bounds;

        static const PrimitiveType FILL_PRIMITIVE_TYPE = PrimitiveType::TrianglesFan;
        static const PrimitiveType OUTLINE_PRIMITIVE_TYPE = PrimitiveType::TrianglesStrip;
        void _update_fill_colors();

        void _update_texture_coords();

        void _update_outline();

        void _update_outline_colors();
};
}

#endif // POLYLINESHAPE_HPP
