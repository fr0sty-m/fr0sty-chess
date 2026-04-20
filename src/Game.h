#pragma once

#include "Board.h"

#include <SFML/Graphics.hpp>

class Game {
  sf::RenderWindow *window;
  Board *board;

public:
  Game();

  void initGame();

  void events();

  void tick();
  void draw();
};
