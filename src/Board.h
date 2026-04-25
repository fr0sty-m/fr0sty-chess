#pragma once

#include "Colors.h"
#include "GameManager.h"
#include "Piece.h"

#include <SFML/Graphics.hpp>

enum class BoardColor { None, Green, Red, Blue, Black };

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
  GameManager *gameManager;

public:
  Board(BoardColor boardColor, GameManager *gm);

  colors boardColors;

  colors selectStyle(BoardColor boardColor);
  void setupPieces();

  void onMousePressed(sf::Vector2i mousePos);
  void onMouseMoved(sf::Vector2i mousePos);
  void onMouseReleased(sf::Vector2i mousePos);

  void handleInput(sf::Vector2i mousePos);

  void update(float dt);
  void draw(sf::RenderWindow *window);
};
