#include "PolylineShape.hpp"

#include <Thor/Vectors.hpp>

namespace sf {

template<class NumberType = float>
Vector2<NumberType> normal(const Vector2<NumberType>& p1, const Vector2<NumberType>& p2)
{
    Vector2<NumberType> normal(p1.y - p2.y, p2.x - p1.x);
    NumberType length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
    if (length != 0.0)
        normal /= length;
    return normal;
}

PolylineShape::PolylineShape()
{
    this->_fill_vertices.setPrimitiveType(FILL_PRIMITIVE_TYPE);
    this->_outline_vertices.setPrimitiveType(OUTLINE_PRIMITIVE_TYPE);
}

PolylineShape::~PolylineShape()
{
    //dtor
}

void PolylineShape::setFillColor(const Color& color) {
    this->_fill_color = color;
    this->_update_fill_colors();
}

void PolylineShape::setOutlineColor(const Color& color) {
    this->_outline_color = color;
    this->_update_outline_colors();
}

void PolylineShape::setOutlineThickness(const float thickness) {
    this->_outline_thickness = thickness;
    this->update();
}

void PolylineShape::setPoint(const int index, const Vector2f& pos) {
    this->_points.at(index) = pos;
    this->update();
}

void PolylineShape::setPoint(const int index, const float x, const float y) {
    this->setPoint(index, Vector2f(x, y));
}

void PolylineShape::setNumPoints(const int num) {
    this->_points.resize(num);
    this->_outline_vertices.resize(num);
    this->_fill_vertices.resize(num);
    this->update();
}

unsigned int PolylineShape::getPointCount() const {
    return this->_points.size();
}

Vector2f PolylineShape::getPoint(const unsigned int index) const {
#ifdef DEBUG
    return this->_points.at(index);  // Bounds checking
#else
    return this->_points[index];  // No bounds checking
#endif
}

// Adapted from sf::Shape
void PolylineShape::draw(RenderTarget& target, RenderStates states) const {
    if (this->_points.size() < 2) return;
    states.transform *= this->getTransform();

    // Render the inside
    states.texture = this->_texture;
    target.draw(this->_fill_vertices, states);

    // Render the outline
    if (this->_outline_thickness != 0)
    {
        states.texture = nullptr;
        target.draw(this->_outline_vertices, states);
    }
}

void PolylineShape::update() {
    // Get the total number of points of the shape
    unsigned int count = this->getPointCount();
    if (count < 3)
    {
        this->_fill_vertices.resize(0);
        this->_outline_vertices.resize(0);
        this->_points.resize(0);
        return;
    }

    this->_fill_vertices.resize(count + 1); // + 1 for center

    // Position
    for (unsigned int i = 0; i < count; ++i)
        this->_fill_vertices[i + 1].position = this->getPoint(i);

    //this->_fill_vertices[count + 1].position = this->_fill_vertices[1].position;

    // Update the bounding rectangle
    this->_fill_vertices[0] = this->_fill_vertices[1]; // so that the result of getBounds() is correct
    this->_inside_bounds = this->_fill_vertices.getBounds();

    // Compute the center and make it the first vertex
    this->_fill_vertices[0].position.x = this->_inside_bounds.left + this->_inside_bounds.width / 2;
    this->_fill_vertices[0].position.y = this->_inside_bounds.top +  this->_inside_bounds.height / 2;

    // Color
    this->_update_fill_colors();

    // Texture coordinates
    this->_update_texture_coords();

    // Outline
    this->_update_outline();
}

void PolylineShape::_update_fill_colors() {
    for (unsigned int i = 0; i < this->_fill_vertices.getVertexCount(); ++i) {
        this->_fill_vertices[i].color = this->_fill_color;
    }
}

void PolylineShape::_update_texture_coords() {
    /*
    for (unsigned int i = 0; i < this->_fill_vertices.getVertexCount(); ++i)
    {
        float xratio = m_insideBounds.width > 0 ? (m_vertices[i].position.x - m_insideBounds.left) / m_insideBounds.width : 0;
        float yratio = m_insideBounds.height > 0 ? (m_vertices[i].position.y - m_insideBounds.top) / m_insideBounds.height : 0;
        m_vertices[i].texCoords.x = m_textureRect.left + m_textureRect.width * xratio;
        m_vertices[i].texCoords.y = m_textureRect.top + m_textureRect.height * yratio;
    }
    */
}

void PolylineShape::_update_outline() {
    unsigned int count = this->_fill_vertices.getVertexCount() - 1;
    this->_outline_vertices.resize(count * 2);

    for (unsigned int i = 0; i < count; ++i) {
        unsigned int index = i + 1;

        // Get the two segments shared by the current point
        Vector2f p0 = (i == 0) ?
                      this->_fill_vertices[count].position :
                      this->_fill_vertices[index - 1].position;
        Vector2f p1 = this->_fill_vertices[index].position;
        Vector2f p2 = this->_fill_vertices[index + 1].position;

        // Compute their normal
        Vector2f n1 = normal(p0, p1);
        Vector2f n2 = normal(p1, p2);

        /*/ Make sure that the normals point towards the outside of the shape
        // (this depends on the order in which the points were defined)
        if (thor::dotProduct(n1, this->_fill_vertices[0].position - p1) > 0)
            n1 = -n1;
        if (thor::dotProduct(n2, this->_fill_vertices[0].position - p1) > 0)
            n2 = -n2;
*/
        // Combine them to get the extrusion direction
        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        Vector2f normal = (n1 + n2) / factor;

        // Update the outline points
        this->_outline_vertices[i * 2 + 0].position = p1;
        this->_outline_vertices[i * 2 + 1].position = p1 + normal * this->_outline_thickness;
    }

    // Duplicate the first point at the end, to close the outline
    //this->_outline_vertices[count * 2 + 0].position = this->_outline_vertices[0].position;
    //this->_outline_vertices[count * 2 + 1].position = this->_outline_vertices[1].position;

    // Update outline colors
    this->_update_outline_colors();

    // Update the shape's bounds
    this->_bounds = this->_outline_vertices.getBounds();
}

void PolylineShape::_update_outline_colors() {
    for (unsigned int i = 0; i < this->_outline_vertices.getVertexCount(); ++i)
        this->_outline_vertices[i].color = this->_outline_color;
}

}

