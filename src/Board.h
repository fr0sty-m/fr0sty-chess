#pragma once

#include "Colors.h"
#include "Piece.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

enum class BoardColor {
  Green,
  Red,
  Blue,
  Black,
};

class Board {
private:
  int tileSize;
  colors boardColors;

  Piece grid[8][8];

  sf::Vector2i selectedSquare;

public:
  Board(BoardColor boardColor);

  colors selectStyle(BoardColor boardColor);

  void handleInput(sf::Vector2i mousePos);

  void setupPieces();

  void draw(sf::RenderWindow *window);
};
