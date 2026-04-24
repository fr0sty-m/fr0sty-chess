#pragma once

#include "Piece.h"

#include <SFML/System/Vector2.hpp>
#include <cmath>

class MoveValidator {
public:
  // Ana giriş kapısı
  static bool isValidMove(sf::Vector2i start, sf::Vector2i end,
                          const Piece grid[8][8]);

private:
  static bool isPathClear(sf::Vector2i start, sf::Vector2i end,
                          const Piece grid[8][8]);
  static bool checkPawn(sf::Vector2i start, sf::Vector2i end, PieceColor color,
                        const Piece grid[8][8]);
  static bool checkRook(sf::Vector2i start, sf::Vector2i end,
                        const Piece grid[8][8]);
  static bool checkKnight(sf::Vector2i start, sf::Vector2i end);
  static bool checkBishop(sf::Vector2i start, sf::Vector2i end,
                          const Piece grid[8][8]);
  static bool checkQueen(sf::Vector2i start, sf::Vector2i end,
                         const Piece grid[8][8]);
  static bool checkKing(sf::Vector2i start, sf::Vector2i end);
};
