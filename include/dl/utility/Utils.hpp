#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace dl {
namespace utils {

inline sf::Vector2f roundVec(const sf::Vector2f& vector) {
    return sf::Vector2f { std::round(vector.x), std::round(vector.y) };
}

} // namespace utils
} // namespace dl