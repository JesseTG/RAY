#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>

#include <SFML/Graphics.hpp>

namespace ray {
using std::vector;
using sf::Event;

template<class T>
T constrain(const T value, const T min, const T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


}



#endif // UTIL_HPP
