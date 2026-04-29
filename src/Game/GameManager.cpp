#include "GameManager.h"

GameManager::GameManager() : currentTurn(PieceColor::White), gameOver(false) {}

void GameManager::switchTurn() {
  currentTurn = (currentTurn == PieceColor::White) ? PieceColor::Black
                                                   : PieceColor::White;
}

void GameManager::addMove(sf::Vector2i from, sf::Vector2i to, Piece p,
                          bool wasCapture) {
  moveHistory.push_back({from, to, p, wasCapture});
}
