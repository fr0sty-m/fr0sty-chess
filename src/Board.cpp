#include "Board.h"

#include "Assets.h"
#include "MoveValidator.h"

#include "Definitions.h"

#include <cmath>

// ================= CONSTRUCTOR =================

Board::Board(BoardColor boardColor, GameManager *gm)
    : boardColors(selectStyle(boardColor)), tileSize(100), gameManager(gm) {

  selectedSquare = {-1, -1};

  Assets::getInstance().loadFont(
      "main", "/usr/share/fonts/TTF/CaskaydiaCoveNerdFont-Bold.ttf");

  Assets::getInstance().loadFont("coord_font",
                                 "/usr/share/fonts/TTF/Hack-Bold.ttf");
}

// ================= STYLE =================

colors Board::selectStyle(BoardColor style) {
  switch (style) {
  case BoardColor::Orange:
    return {ORANGE_DARK, ORANGE_LIGHT};
  case BoardColor::Green:
    return {GREEN_DARK, GREEN_LIGHT};
  case BoardColor::Red:
    return {RED_DARK, RED_LIGHT};
  case BoardColor::Blue:
    return {BLUE_DARK, BLUE_LIGHT};
  case BoardColor::Black:
    return {BLACK_DARK, BLACK_LIGHT};
  default:
    return {RED_DARK, RED_LIGHT};
  }
}

// ================= INPUT =================

void Board::onMousePressed(sf::Vector2i mousePos) {
  mousePressed = true;
  dragStartSquare = {mousePos.x / tileSize, mousePos.y / tileSize};
}

void Board::onMouseMoved(sf::Vector2i mousePos) {
  if (!mousePressed)
    return;

  if (!isDragging &&
      (std::abs(mousePos.x - dragStartSquare.x * tileSize) > 5 ||
       std::abs(mousePos.y - dragStartSquare.y * tileSize) > 5)) {

    int col = dragStartSquare.x, row = dragStartSquare.y;

    if (col < 0 || col >= 8 || row < 0 || row >= 8 ||
        grid[row][col].type == PieceType::None)
      return;

    isDragging = true;
    draggedPiece = grid[row][col];
    grid[row][col] = {PieceType::None, PieceColor::None};

    dragOffset = {(float)mousePos.x - col * tileSize,
                  (float)mousePos.y - row * tileSize};
  }

  if (isDragging)
    currentMousePos = (sf::Vector2f)mousePos;
}

void Board::onMouseReleased(sf::Vector2i mousePos) {
  if (!mousePressed)
    return;

  int col = mousePos.x / tileSize, row = mousePos.y / tileSize;

  if (isDragging) {

    grid[dragStartSquare.y][dragStartSquare.x] = draggedPiece;

    if (col >= 0 && col < 8 && row >= 0 && row < 8 &&
        draggedPiece.color == gameManager->getCurrentTurn() &&
        MoveValidator::isValidMove(dragStartSquare, {col, row}, grid)) {

      grid[dragStartSquare.y][dragStartSquare.x] = {PieceType::None,
                                                    PieceColor::None};
      grid[row][col] = draggedPiece;

      gameManager->addMove(dragStartSquare, {col, row}, draggedPiece, false);
      gameManager->switchTurn();
    }

    isDragging = false;
    selectedSquare = {-1, -1};
  } else {
    handleInput(mousePos);
  }

  mousePressed = false;
}

// ================= CLICK MOVE =================

void Board::handleInput(sf::Vector2i mousePos) {
  int col = mousePos.x / tileSize, row = mousePos.y / tileSize;

  if (col < 0 || col >= 8 || row < 0 || row >= 8)
    return;

  if (selectedSquare.x == -1) {
    if (grid[row][col].type != PieceType::None &&
        grid[row][col].color == gameManager->getCurrentTurn()) {
      selectedSquare = {col, row};
    }
  } else {

    if (selectedSquare.x == col && selectedSquare.y == row) {
      selectedSquare = {-1, -1};
      return;
    }

    if (MoveValidator::isValidMove(selectedSquare, {col, row}, grid)) {

      movingPiece.piece = grid[selectedSquare.y][selectedSquare.x];
      movingPiece.startPos = {(float)selectedSquare.x * tileSize,
                              (float)selectedSquare.y * tileSize};
      movingPiece.endPos = {(float)col * tileSize, (float)row * tileSize};
      movingPiece.progress = 0.f;
      movingPiece.active = true;

      grid[selectedSquare.y][selectedSquare.x] = {PieceType::None,
                                                  PieceColor::None};

      bool isCapture = (grid[row][col].type != PieceType::None);

      gameManager->addMove(selectedSquare, {col, row}, movingPiece.piece,
                           isCapture);
      gameManager->switchTurn();
    }

    selectedSquare = {-1, -1};
  }
}

// ================= UPDATE =================

void Board::update(float dt) {
  if (!movingPiece.active)
    return;

  movingPiece.progress += dt * 8.f;

  if (movingPiece.progress >= 1.f) {
    movingPiece.progress = 1.f;
    movingPiece.active = false;

    int col = movingPiece.endPos.x / tileSize;
    int row = movingPiece.endPos.y / tileSize;

    grid[row][col] = movingPiece.piece;
  }
}

// ================= SETUP =================

void Board::setupPieces() {
  PieceType backRow[] = {
      PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
      PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook};

  for (int i = 0; i < 8; i++) {
    grid[0][i] = {backRow[i], PieceColor::Black};
    grid[1][i] = {PieceType::Pawn, PieceColor::Black};

    grid[7][i] = {backRow[i], PieceColor::White};
    grid[6][i] = {PieceType::Pawn, PieceColor::White};
  }
}

// ================= HELPERS =================

std::string Board::getPieceKey(const Piece &p) {
  std::string color = (p.color == PieceColor::Black) ? "black_" : "white_";

  switch (p.type) {
  case PieceType::Pawn:
    return color + "pawn";
  case PieceType::Rook:
    return color + "rook";
  case PieceType::Knight:
    return color + "knight";
  case PieceType::Bishop:
    return color + "bishop";
  case PieceType::Queen:
    return color + "queen";
  case PieceType::King:
    return color + "king";
  default:
    return "";
  }
}

void Board::drawUI(sf::RenderWindow *window, const sf::Font &font) {

  // ===== PANEL =====
  sf::RectangleShape uiArea(sf::Vector2f(480, 800));
  uiArea.setPosition({800, 0});
  uiArea.setFillColor(sf::Color(26, 27, 38));
  window->draw(uiArea);

  // ===== BLACK TEXT =====
  sf::Text blackText(font);
  blackText.setString("BLACK");
  blackText.setCharacterSize(50);
  blackText.setStyle(sf::Text::Bold);
  blackText.setFillColor(sf::Color(20, 20, 20));
  blackText.setPosition({950, 80});
  window->draw(blackText);

  // ===== WHITE TEXT =====
  sf::Text whiteText(font);
  whiteText.setString("WHITE");
  whiteText.setCharacterSize(50);
  whiteText.setStyle(sf::Text::Bold);
  whiteText.setFillColor(sf::Color::White);
  whiteText.setPosition({950, 650});
  window->draw(whiteText);

  // ===== TURN INDICATOR =====
  sf::CircleShape indicator(15);
  indicator.setOutlineThickness(2);
  indicator.setOutlineColor(sf::Color(122, 162, 247));
  indicator.setFillColor(sf::Color(158, 206, 106));

  if (gameManager->getCurrentTurn() == PieceColor::Black) {
    indicator.setPosition({910, 96});
  } else {
    indicator.setPosition({910, 668});
  }

  window->draw(indicator);
}

void Board::drawTile(sf::RenderWindow *window, int col, int row) {
  sf::RectangleShape square({(float)tileSize, (float)tileSize});
  square.setPosition({col * (float)tileSize, row * (float)tileSize});
  square.setFillColor(((row + col) % 2 == 0) ? boardColors.white
                                             : boardColors.black);
  window->draw(square);
}

void Board::drawPiece(sf::RenderWindow *window, const Piece &p, int col,
                      int row) {
  if (p.type == PieceType::None)
    return;

  auto &assets = Assets::getInstance();
  const sf::Texture &tex = assets.getTexture(getPieceKey(p));

  sf::Sprite sprite(tex);
  float scale = (float)tileSize / tex.getSize().x;

  sprite.setScale({scale, scale});
  sprite.setPosition({col * (float)tileSize, row * (float)tileSize});

  window->draw(sprite);
}

void Board::drawDragged(sf::RenderWindow *window) {
  if (!isDragging)
    return;

  auto &assets = Assets::getInstance();
  const sf::Texture &tex = assets.getTexture(getPieceKey(draggedPiece));

  sf::Sprite sprite(tex);
  float scale = (float)tileSize / tex.getSize().x;

  sprite.setScale({scale, scale});
  sprite.setPosition(currentMousePos - dragOffset);

  window->draw(sprite);
}

void Board::drawMoving(sf::RenderWindow *window) {
  if (!movingPiece.active)
    return;

  auto &assets = Assets::getInstance();
  const sf::Texture &tex = assets.getTexture(getPieceKey(movingPiece.piece));

  sf::Sprite sprite(tex);
  float scale = (float)tileSize / tex.getSize().x;

  sprite.setScale({scale, scale});

  sf::Vector2f pos =
      movingPiece.startPos +
      (movingPiece.endPos - movingPiece.startPos) * movingPiece.progress;

  sprite.setPosition(pos);
  window->draw(sprite);
}

void Board::drawCoords(sf::RenderWindow *window, const sf::Font &font) {

  for (int col = 0; col < 8; col++) {
    sf::Text t(font);
    t.setString(std::string(1, 'a' + col));
    t.setCharacterSize(16);

    bool light = ((7 + col) % 2 == 0);
    t.setFillColor(light ? sf::Color::Black : sf::Color::White);

    t.setPosition({col * tileSize + tileSize - 18.f, 8 * tileSize - 20.f});
    window->draw(t);
  }

  for (int row = 0; row < 8; row++) {
    sf::Text t(font);
    t.setString(std::string(1, '8' - row));
    t.setCharacterSize(16);

    bool light = ((row + 0) % 2 == 0);
    t.setFillColor(light ? sf::Color::Black : sf::Color::White);

    t.setPosition({4.f, row * tileSize + 4.f});
    window->draw(t);
  }
}

// ================= DRAW =================

void Board::draw(sf::RenderWindow *window) {

  auto &assets = Assets::getInstance();
  const sf::Font &coordFont = assets.getFont("coord_font");

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {

      drawTile(window, col, row);

      if (isDragging && dragStartSquare.x == col && dragStartSquare.y == row)
        continue;

      drawPiece(window, grid[row][col], col, row);
    }
  }

  drawDragged(window);
  drawMoving(window);
  drawCoords(window, coordFont);
  drawUI(window, coordFont);
}
