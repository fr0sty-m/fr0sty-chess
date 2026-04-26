#pragma once

#include "Colors.h"
#include "GameManager.h"
#include "Piece.h"

#include <SFML/Graphics.hpp>

// ================= BOARD COLOR ======================
enum class BoardColor {
  None,
  Orange,
  Green,
  Red,
  Blue,
  Black,
  Dark,
  Purple,
  Gray,
  Chesscom,
  Teal,
  Lichess,
};
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

  std::string pieceStyle;

  GameManager *gameManager;

  std::vector<sf::Vector2i> legalMoves;

  // ===== HELPERS =====
  std::string getPieceKey(const Piece &p);
  void drawTile(sf::RenderWindow *window, int col, int row);
  void drawPiece(sf::RenderWindow *window, const Piece &p, int col, int row);
  void drawDragged(sf::RenderWindow *window);
  void drawMoving(sf::RenderWindow *window);
  void drawCoords(sf::RenderWindow *window, const sf::Font &font);
  void drawUI(sf::RenderWindow *window, const sf::Font &font);
  void drawHighlight(sf::RenderWindow *window);
  void drawLegalMoves(sf::RenderWindow *window);

public:
  Board(std::string boardColor, std::string pieceStyle, GameManager *gm);

  colors boardColors;

  colors selectStyle(BoardColor boardColor);
  static BoardColor themeToColor(const std::string &theme);

  void setTheme(BoardColor newColor);
  void setPieceStyle(const std::string &style);

  void setupPieces();

  void onMousePressed(sf::Vector2i mousePos);
  void onMouseMoved(sf::Vector2i mousePos);
  void onMouseReleased(sf::Vector2i mousePos);

  void handleInput(sf::Vector2i mousePos);

  void update(float dt);
  void draw(sf::RenderWindow *window);
};
