#pragma once

#include "Colors.h"
#include "GameManager.h"
#include "Piece.h"

#include <SFML/Graphics.hpp>

// ================= BOARD COLOR ======================
enum class BoardColor { None, Orange, Green, Red, Blue, Black };
// ====================================================

class Board {
private:
  // ================= MOVING PIECE ======================
  struct MovingPiece {
    Piece piece;
    sf::Vector2f startPos, endPos;
    float progress = 0.f;
    bool active = false;
  };
  // ================= MOVING PIECE ======================

  int tileSize;
  Piece grid[8][8];

  sf::Vector2i selectedSquare;
  bool mousePressed = false, isDragging = false;

  Piece draggedPiece;
  sf::Vector2i dragStartSquare;
  sf::Vector2f dragOffset, currentMousePos;

  MovingPiece movingPiece;

  GameManager *gameManager;

  // ===== HELPERS =====
  std::string getPieceKey(const Piece &p);
  void drawTile(sf::RenderWindow *window, int col, int row);
  void drawPiece(sf::RenderWindow *window, const Piece &p, int col, int row);
  void drawDragged(sf::RenderWindow *window);
  void drawMoving(sf::RenderWindow *window);
  void drawCoords(sf::RenderWindow *window, const sf::Font &font);
  void drawUI(sf::RenderWindow *window, const sf::Font &font);

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
