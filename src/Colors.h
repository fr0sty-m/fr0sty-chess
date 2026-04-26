#pragma once

#include <SFML/Graphics.hpp>

// colors struct for making the project easier to handle
struct colors {
  sf::Color black, white;

  colors(sf::Color black, sf::Color white) : black(black), white(white) {}
};
