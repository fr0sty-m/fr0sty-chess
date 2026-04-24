#pragma once

#include "Colors.h"
#include "Piece.h"

#include <SFML/Graphics.hpp>

enum class BoardColor { Green, Red, Blue, Black };

class Board {
private:
  struct MovingPiece {
    Piece piece;
    sf::Vector2f startPos, endPos;
    float progress = 0.f;
    bool active = false;
  };

  int tileSize;
  Piece grid[8][8];
  sf::Vector2i selectedSquare;
  bool mousePressed = false, isDragging = false;
  Piece draggedPiece;
  sf::Vector2i dragStartSquare;
  sf::Vector2f dragOffset, currentMousePos;
  MovingPiece movingPiece;

public:
  colors boardColors;
  Board(BoardColor boardColor);
  colors selectStyle(BoardColor boardColor);
  void onMousePressed(sf::Vector2i mousePos);
  void onMouseMoved(sf::Vector2i mousePos);
  void onMouseReleased(sf::Vector2i mousePos);
  void handleInput(sf::Vector2i mousePos);
  void update(float dt);
  void setupPieces();
  void draw(sf::RenderWindow *window);
};
