#pragma once

#include "Colors.h"
#include "Piece.h"

#include <SFML/Graphics.hpp>

enum class BoardColor {
  Green,
  Red,
  Blue,
  Black,
};

class Board {
private:
  struct MovingPiece {
    Piece piece;
    sf::Vector2f startPos;
    sf::Vector2f endPos;
    float progress = 0.f;
    bool active = false;
  };

private:
  int tileSize;
  colors boardColors;

  Piece grid[8][8];

  sf::Vector2i selectedSquare;

  // input state
  bool mousePressed = false;

  // drag state
  bool isDragging = false;
  Piece draggedPiece;
  sf::Vector2i dragStartSquare;
  sf::Vector2f dragOffset;
  sf::Vector2f currentMousePos;

  MovingPiece movingPiece;

public:
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
