#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>
#include <vector>
#include <functional>
#include <type_traits>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "config.hpp"

#include "config.hpp"

namespace ray {
using std::vector;
using std::function;
using sf::Event;
using sf::Vector2;

using boost::optional;
using boost::variant;

/**
 * Type alias for @c boost::optional<boost::variant<Your, Types, Here>>, for
 * passing to a Lua function a varying amount of arguments with differing types.
 */
template<typename...Types> using optional_variant = optional<variant<Types...>>;

/**
 * Given a number, constrains it to a range.
 *
 * @param value The number to constrain
 * @param min Returned if @c value is less than this
 * @param max Returned if @c value is more than this
 * @tparam The numerical type we're operating on (though I guess it could
 * really be anything that defines @c operator< and @c operator>).
 *
 * @return @c value if it falls within the range set by @c min and @c max, the
 * closest value otherwise.
 */
template<class NumberType>
inline NumberType constrain(const NumberType value, const NumberType min, const NumberType max) noexcept {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/**
 * Returns the dot product of two @c sf::Vector2 objects.
 *
 * @param a The first @c sf::Vector2 we're operating on
 * @param b The second @c sf::Vector2 we're operating on
 * @tparam NumberType The numerical type stored in the @c sf::Vector
 *
 * @return The dot product of vectors @c a and @c b
 */
template<class NumberType = float>
inline NumberType dot(const Vector2<NumberType>& a, const Vector2<NumberType>& b) noexcept {
    return (a.x * b.x) + (a.y * b.y);
}

/**
 * Converts a number specified in degrees to radians (e.g. 180 to pi)
 *
 * @param x The number to convert
 * @tparam NumberType the numerical type of @c x
 *
 * @return @c x converted from degrees to radians
 */
template<class NumberType>
inline NumberType toRadians(const NumberType x) noexcept {
    return x * (PI / 180.0);
}

/**
 * Converts a number specified in radians to degrees (e.g. pi to 180)
 *
 * @param x The number to convert
 * @tparam NumberType the numerical type of @c x
 *
 * @return @c x converted from radians to degrees
 */
template<class NumberType>
inline NumberType toDegrees(const NumberType x) noexcept {
    return x * (180.0 / PI);
}

/**
 * Returns the square of a given number.
 *
 * @param x The number to be squared
 * @tparam NumberType The numerical type of @c x
 */
template<class NumberType>
inline NumberType square(const NumberType x) noexcept {
    return x * x;
}

/**
 * Converts a @c sf::Vector2 to a @c b2Vec2
 *
 * @param vec The @c sf::Vector2 to convert
 * @tparam NumberType The numerical type of @c vec's values
 *
 * @return A @c b2Vec2 constructed from @c vec's values
 */
template<class NumberType>
inline b2Vec2 sfVecToB2Vec(const Vector2<NumberType>& vec) noexcept {
    return b2Vec2(vec.x, vec.y);
}

template<class ContainerType, class NumberType = float>
inline vector<b2Vec2> sfVecsToB2Vecs(const ContainerType& container) noexcept {
    vector<b2Vec2> vecs;
    vecs.reserve(container.size());
    for (const Vector2<NumberType>& v : container) {
        vecs.push_back(b2Vec2(v.x, v.y));
    }

    return vecs;
}


/**
 * Converts a @c b2Vec2 to a @c sf::Vector2
 *
 * @param vec The @c sf::Vector2 to convert
 * @tparam NumberType The numerical type of @c vec's values (@c float by default)
 *
 * @return A @c sf::Vector<NumberType> constructed from @c vec's values
 */
template<class NumberType = float>
inline Vector2<NumberType> b2VecToSfVec(const b2Vec2& vec) noexcept {
    return Vector2<NumberType>(vec.x, vec.y);
}

/**
 * Returns the sign of a number; i.e. 1 if the number is positive, -1 is the
 * number is negative, 0 if neither.
 *
 * @tparam The type of the number
 */
template<class NumberType>
inline NumberType sign(const NumberType num) noexcept {
    static_assert(std::is_signed<NumberType>::value, "Can't use an unsigned type");
    if (num > 0) return 1;
    if (num < 0) return -1;
    return 0;
}

/**
 * Returns a lambda function that calls the default constructor with on a
 * given class.
 *
 * @tparam T The type that the returned lambda will default-construct
 */
template<class T>
inline function<T(void)> getDefaultConstructorLambda() noexcept {
    return []() {
        return T();
    };
}

/**
 * Returns a lambda function that calls the default constructor with new on a
 * given class.
 *
 * @tparam T The type that the returned lambda will default-construct
 */
template<class T>
inline function<T*(void)> getNewDefaultConstructorLambda() noexcept {
    return []() {
        return new T;
    };
}
}

#endif // UTIL_HPP
