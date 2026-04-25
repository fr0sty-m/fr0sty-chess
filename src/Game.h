#pragma once

#include "Board.h"
#include "GameManager.h"

#include <SFML/Graphics.hpp>

class Game {
  sf::RenderWindow *window;
  Board *board;
  GameManager gm;

public:
  Game();

  void initGame();

  void events();

  void tick();
  void draw();
};
