#pragma once

#include "Piece.h"

#include <vector>

struct Move {
  sf::Vector2i from;
  sf::Vector2i to;
  Piece piece;
  bool captured;
};

class GameManager {
public:
  GameManager();

  PieceColor getCurrentTurn() const { return currentTurn; }
  void switchTurn();

  // Hamle geçmişi yönetimi
  void addMove(sf::Vector2i from, sf::Vector2i to, Piece p, bool wasCapture);
  const std::vector<Move> &getHistory() const { return moveHistory; }

  // Oyun durumu (Şu an basit, sonra geliştireceğiz)
  bool isGameOver() const { return gameOver; }

private:
  PieceColor currentTurn;
  std::vector<Move> moveHistory;
  bool gameOver;
};
