#pragma once

#include "Utils/Colors.h"
#include "Utils/Piece.h"
#include <vector>

struct BoardData {
  //===============================================
  struct MovingPiece {
    Piece piece;
    sf::Vector2f startPos, endPos;
    float progress = 0.f;
    bool active = false;
  };
  //===============================================

  MovingPiece movingPiece;

  int tileSize = 100;
  Piece grid[8][8];
  sf::Vector2i selectedSquare = {-1, -1};
  std::vector<sf::Vector2i> legalMoves;

  colors boardColors;
  std::string pieceStyle;

  bool isDragging = false;
  Piece draggedPiece;
  sf::Vector2f currentMousePos;
  sf::Vector2f dragOffset;

  BoardData(int tileSize, const std::string &style, colors bColors)
      : tileSize(tileSize), selectedSquare{-1, -1}, boardColors(bColors),
        pieceStyle(style), isDragging(false) {}
};
