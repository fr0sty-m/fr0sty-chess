#pragma once

#include "BoardRenderer.h"
#include "Game/GameManager.h"
#include "Utils/BoardData.h"
#include "Utils/Definitions.h"

#include <SFML/Graphics.hpp>

class Board {
private:
  BoardData boardData;
  BoardRenderer *renderer;

  GameManager *gameManager;

public:
  Board(std::string boardColor, std::string pieceStyle, GameManager *gm);
  ~Board();

  void setupPieces();
  void update(float dt);

  void handleMousePressed(sf::Vector2i mousePos);
  void handleMouseMoved(sf::Vector2i mousePos);
  void handleMouseReleased(sf::Vector2i mousePos);

  void draw(sf::RenderWindow *window);

  void setTheme(BoardColor newColor);
  void setPieceStyle(const std::string &style);
};
