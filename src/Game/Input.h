#pragma once

#include "GameManager.h"
#include "Utils/BoardData.h"

#include <SFML/System/Vector2.hpp>

struct Input {
  static bool mousePressed;
  static sf::Vector2i dragStartSquare;
  static const int DRAG_THRESHOLD = 70;

  static void onMousePressed(sf::Vector2i mousePos, BoardData &boardData);
  static void onMouseMoved(sf::Vector2i mousePos, BoardData &boardData);
  static void onMouseReleased(sf::Vector2i mousePos, BoardData &boardData,
                              GameManager *gameManager);

  static void handleInput(sf::Vector2i mousePos, BoardData &boardData,
                          GameManager *gameManager);
};
