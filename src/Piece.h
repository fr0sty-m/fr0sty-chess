#pragma once

#include <SFML/Graphics.hpp>

enum class PieceType { None, Pawn, Knight, Bishop, Rook, Queen, King };
enum class PieceColor { None, White, Black };

struct Piece {
  PieceType type = PieceType::None;
  PieceColor color = PieceColor::None;

  Piece() {}
  Piece(PieceType t, PieceColor c) : type(t), color(c) {}
};
