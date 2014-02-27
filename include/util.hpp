#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>

#include <SFML/Graphics.hpp>

namespace ray {
using std::vector;
using sf::Event;
using sf::Vector2;

template<class NumberType>
inline NumberType constrain(const NumberType value, const NumberType min, const NumberType max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template<class NumberType>
inline NumberType dot(const Vector2<NumberType>& a, const Vector2<NumberType>& b) {
    return (a.x * b.x) + (a.y * b.y);
}

template<class NumberType>
inline NumberType toRadians(const NumberType x) {
    return x * (M_PI / 180.0);
}

template<class NumberType>
inline NumberType toDegrees(const NumberType x) {
    return x * (180.0 / M_PI);
}

template<class NumberType>
inline NumberType square(const NumberType x) {
    return x * x;
}


}



#endif // UTIL_HPP
