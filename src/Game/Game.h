#pragma once

#include "Board/Board.h"
#include "Config/ThemeManager.h"
#include "GameManager.h"

#include <SFML/Graphics.hpp>

class Game {
private:
  sf::RenderWindow *window;
  Board *board;
  GameManager gm;
  ThemeManager themeManager;

public:
  Game();

  void initGame();

  void events();

  void tick();
  void draw();
};
