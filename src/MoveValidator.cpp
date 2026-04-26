#include "MoveValidator.h"

bool MoveValidator::isValidMove(sf::Vector2i start, sf::Vector2i end,
                                const Piece grid[8][8]) {
  if (end.x < 0 || end.x >= 8 || end.y < 0 || end.y >= 8)
    return false;
  if (start == end)
    return false;

  Piece movingPiece = grid[start.y][start.x];
  Piece targetPiece = grid[end.y][end.x];

  // Eğer hedef karede bir Şah varsa, o kareye gidilemez.
  if (targetPiece.type == PieceType::King) {
    return false;
  }

  // Kendi taşını yeme kontrolü
  if (targetPiece.type != PieceType::None &&
      targetPiece.color == movingPiece.color)
    return false;

  switch (movingPiece.type) {
  case PieceType::Pawn:
    return checkPawn(start, end, movingPiece.color, grid);
  case PieceType::Rook:
    return checkRook(start, end, grid);
  case PieceType::Knight:
    return checkKnight(start, end);
  case PieceType::Bishop:
    return checkBishop(start, end, grid);
  case PieceType::Queen:
    return checkQueen(start, end, grid);
  case PieceType::King:
    return checkKing(start, end);
  default:
    return false;
  }
}

std::vector<sf::Vector2i> MoveValidator::getLegalMoves(sf::Vector2i start,
                                                       const Piece grid[8][8]) {

  std::vector<sf::Vector2i> moves;

  Piece p = grid[start.y][start.x];
  if (p.type == PieceType::None)
    return moves;

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {

      sf::Vector2i end = {x, y};

      if (isValidMove(start, end, grid)) {
        moves.push_back(end);
      }
    }
  }

  return moves;
}

bool MoveValidator::isPathClear(sf::Vector2i start, sf::Vector2i end,
                                const Piece grid[8][8]) {
  int dx = (end.x == start.x) ? 0 : (end.x > start.x ? 1 : -1);
  int dy = (end.y == start.y) ? 0 : (end.y > start.y ? 1 : -1);
  int currX = start.x + dx;
  int currY = start.y + dy;

  while (currX != end.x || currY != end.y) {
    if (grid[currY][currX].type != PieceType::None)
      return false;
    currX += dx;
    currY += dy;
  }
  return true;
}

bool MoveValidator::checkPawn(sf::Vector2i start, sf::Vector2i end,
                              PieceColor color, const Piece grid[8][8]) {
  int dx = end.x - start.x;
  int dy = end.y - start.y;
  int direction = (color == PieceColor::White) ? -1 : 1;
  int startRow = (color == PieceColor::White) ? 6 : 1;

  if (dx == 0) {
    if (grid[end.y][end.x].type == PieceType::None) {
      if (dy == direction)
        return true;
      if (start.y == startRow && dy == 2 * direction) {
        return grid[start.y + direction][start.x].type == PieceType::None;
      }
    }
  } else if (std::abs(dx) == 1 && dy == direction) {
    return grid[end.y][end.x].type != PieceType::None &&
           grid[end.y][end.x].color != color;
  }
  return false;
}

bool MoveValidator::checkKnight(sf::Vector2i start, sf::Vector2i end) {
  int dx = std::abs(end.x - start.x);
  int dy = std::abs(end.y - start.y);
  return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

bool MoveValidator::checkRook(sf::Vector2i start, sf::Vector2i end,
                              const Piece grid[8][8]) {
  if (start.x != end.x && start.y != end.y)
    return false;
  return isPathClear(start, end, grid);
}

bool MoveValidator::checkBishop(sf::Vector2i start, sf::Vector2i end,
                                const Piece grid[8][8]) {
  if (std::abs(end.x - start.x) != std::abs(end.y - start.y))
    return false;
  return isPathClear(start, end, grid);
}

bool MoveValidator::checkQueen(sf::Vector2i start, sf::Vector2i end,
                               const Piece grid[8][8]) {
  return checkRook(start, end, grid) || checkBishop(start, end, grid);
}

bool MoveValidator::checkKing(sf::Vector2i start, sf::Vector2i end) {
  return std::abs(end.x - start.x) <= 1 && std::abs(end.y - start.y) <= 1;
}
