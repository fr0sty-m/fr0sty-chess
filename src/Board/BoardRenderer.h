#pragma once

#include "Game/GameManager.h"
#include "Utils/BoardData.h"
#include "Utils/Colors.h"

#include <SFML/Graphics.hpp>

class BoardRenderer {
private:
  GameManager *gameManager;
  const BoardData &boardData;

public:
  BoardRenderer(GameManager *gm, const BoardData &boardData);

  void draw(sf::RenderWindow *window);

private:
  void drawGrid(sf::RenderWindow *window, const colors &bColors);
  void drawPieces(sf::RenderWindow *window, const std::string &style);
  void drawDragged(sf::RenderWindow *window);

  std::string getPieceKey(const Piece &p);
  void drawTile(sf::RenderWindow *window, int col, int row);
  void drawPiece(sf::RenderWindow *window, const Piece &p, int col, int row);
  void drawMoving(sf::RenderWindow *window);
  void drawCoords(sf::RenderWindow *window, const sf::Font &font);
  void drawUI(sf::RenderWindow *window, const sf::Font &font);
  void drawHighlight(sf::RenderWindow *window);
  void drawLegalMoves(sf::RenderWindow *window);
};
