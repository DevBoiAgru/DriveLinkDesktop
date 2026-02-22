#pragma once

#include "InputState.hpp"
#include "network/Server.hpp"
#include "ui/button.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

namespace dl {

namespace consts {

constexpr unsigned int IMAGE_W = 970;
constexpr unsigned int IMAGE_H = 560;
constexpr float aspect_ratio = float(IMAGE_W) / IMAGE_H;

constexpr float TITLEBAR_HEIGHT = 45;

} // namespace consts

void enforceAspectRatio(sf::RenderWindow& window, unsigned w, unsigned h) {
    unsigned newW = w;
    unsigned newH = static_cast<unsigned>(w / consts::aspect_ratio);

    if (newH > h) {
        newH = h;
        newW = static_cast<unsigned>(h * consts::aspect_ratio);
    }

    window.setSize({ newW, newH });
}

} // namespace dl