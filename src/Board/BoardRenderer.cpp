#include "BoardRenderer.h"

#include "Assets/Assets.h"
#include "Utils/Definitions.h"

BoardRenderer::BoardRenderer(GameManager *gm, const BoardData &boardData)
    : gameManager(gm), boardData(boardData) {}

void BoardRenderer::draw(sf::RenderWindow *window) {
  auto &assets = Assets::getInstance();
  const sf::Font &coordFont = assets.getFont("coord_font");

  // 1. TILE + HIGHLIGHT + PIECES (layer together)
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {

      drawTile(window, col, row);

      if (!boardData.isDragging && boardData.selectedSquare.x == col &&
          boardData.selectedSquare.y == row &&
          boardData.grid[row][col].type != PieceType::None) {

        sf::RectangleShape highlight(
            sf::Vector2f((float)boardData.tileSize, (float)boardData.tileSize));

        highlight.setPosition(
            {col * (float)boardData.tileSize, row * (float)boardData.tileSize});

        highlight.setFillColor(HIGHTLIGHT_COLOR);

        window->draw(highlight);
      }

      drawLegalMoves(window);
      drawPiece(window, boardData.grid[row][col], col, row);
    }
  }

  // 2. overlays
  drawMoving(window);

  // 3. UI + coords
  drawCoords(window, coordFont);
  drawUI(window, coordFont);
  drawDragged(window);
}

void BoardRenderer::drawHighlight(sf::RenderWindow *window) {
  if (boardData.selectedSquare.x < 0 || boardData.selectedSquare.y < 0)
    return;

  if (boardData.isDragging)
    return;

  if (boardData.grid[boardData.selectedSquare.y][boardData.selectedSquare.x]
          .type == PieceType::None)
    return;

  sf::RectangleShape highlight(
      sf::Vector2f((float)boardData.tileSize, (float)boardData.tileSize));

  highlight.setPosition(
      {boardData.selectedSquare.x * (float)boardData.tileSize,
       boardData.selectedSquare.y * (float)boardData.tileSize});

  highlight.setFillColor(HIGHTLIGHT_COLOR);

  window->draw(highlight);
}

void BoardRenderer::drawUI(sf::RenderWindow *window, const sf::Font &font) {
  // ===== PANEL =====
  sf::RectangleShape uiAreaB(sf::Vector2f(480, 400));
  uiAreaB.setPosition({800, 0});
  uiAreaB.setFillColor(sf::Color(100, 100, 100));
  window->draw(uiAreaB);

  sf::RectangleShape uiAreaW(sf::Vector2f(480, 400));
  uiAreaW.setPosition({800, 400});
  uiAreaW.setFillColor(sf::Color(26, 27, 38));
  window->draw(uiAreaW);

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

void BoardRenderer::drawTile(sf::RenderWindow *window, int col, int row) {
  sf::RectangleShape square(
      {(float)boardData.tileSize, (float)boardData.tileSize});
  square.setPosition(
      {col * (float)boardData.tileSize, row * (float)boardData.tileSize});
  square.setFillColor(((row + col) % 2 == 0) ? boardData.boardColors.white
                                             : boardData.boardColors.black);
  window->draw(square);
}

void BoardRenderer::drawPiece(sf::RenderWindow *window, const Piece &p, int col,
                              int row) {
  if (p.type == PieceType::None)
    return;

  auto &assets = Assets::getInstance();
  const sf::Texture &tex = assets.getTexture(getPieceKey(p));

  sf::Sprite sprite(tex);
  float scale = (float)boardData.tileSize / tex.getSize().x;

  sprite.setScale({scale, scale});
  sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});

  sprite.setPosition({col * boardData.tileSize + boardData.tileSize / 2.f,
                      row * boardData.tileSize + boardData.tileSize / 2.f});

  window->draw(sprite);
}

void BoardRenderer::drawDragged(sf::RenderWindow *window) {
  if (!boardData.isDragging)
    return;

  auto &assets = Assets::getInstance();
  const sf::Texture &tex =
      assets.getTexture(getPieceKey(boardData.draggedPiece));

  sf::Sprite sprite(tex);

  sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});

  float scale = (float)boardData.tileSize / tex.getSize().x;
  sprite.setScale({scale, scale});

  sprite.setPosition(boardData.currentMousePos);

  window->draw(sprite);
}

void BoardRenderer::drawMoving(sf::RenderWindow *window) {
  if (!boardData.movingPiece.active)
    return;

  auto &assets = Assets::getInstance();
  const sf::Texture &tex =
      assets.getTexture(this->getPieceKey(boardData.movingPiece.piece));

  sf::Sprite sprite(tex);

  sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});

  float scale = (float)boardData.tileSize / tex.getSize().x;
  sprite.setScale({scale, scale});

  sf::Vector2f start =
      boardData.movingPiece.startPos +
      sf::Vector2f(boardData.tileSize / 2.f, boardData.tileSize / 2.f);

  sf::Vector2f end =
      boardData.movingPiece.endPos +
      sf::Vector2f(boardData.tileSize / 2.f, boardData.tileSize / 2.f);

  sf::Vector2f pos = start + (end - start) * boardData.movingPiece.progress;

  sprite.setPosition(pos);

  window->draw(sprite);
}

void BoardRenderer::drawCoords(sf::RenderWindow *window, const sf::Font &font) {
  for (int col = 0; col < 8; col++) {
    sf::Text t(font);
    t.setString(std::string(1, 'a' + col));
    t.setCharacterSize(16);

    bool light = ((7 + col) % 2 == 0);
    sf::Color tileColor =
        light ? boardData.boardColors.white : boardData.boardColors.black;

    int luminance =
        0.299f * tileColor.r + 0.587f * tileColor.g + 0.114f * tileColor.b;

    t.setFillColor(luminance > 128 ? sf::Color::Black : sf::Color::White);

    t.setPosition({col * boardData.tileSize + boardData.tileSize - 18.f,
                   8 * boardData.tileSize - 20.f});
    window->draw(t);
  }

  for (int row = 0; row < 8; row++) {
    sf::Text t(font);
    t.setString(std::string(1, '8' - row));
    t.setCharacterSize(16);

    bool light = ((row + 0) % 2 == 0);
    sf::Color tileColor =
        light ? boardData.boardColors.white : boardData.boardColors.black;

    int luminance =
        0.299f * tileColor.r + 0.587f * tileColor.g + 0.114f * tileColor.b;

    t.setFillColor(luminance > 128 ? sf::Color::Black : sf::Color::White);

    t.setPosition({4.f, row * boardData.tileSize + 4.f});
    window->draw(t);
  }
}

void BoardRenderer::drawLegalMoves(sf::RenderWindow *window) {
  for (auto &pos : boardData.legalMoves) {

    bool isCapture = boardData.grid[pos.y][pos.x].type != PieceType::None;

    if (isCapture) {
      sf::CircleShape ring(boardData.tileSize / 2.f - 6.f);
      ring.setPosition(
          {pos.x * boardData.tileSize + 6.f, pos.y * boardData.tileSize + 6.f});

      ring.setFillColor(sf::Color::Transparent);
      ring.setOutlineThickness(4.f);
      ring.setOutlineColor(sf::Color(120, 255, 120, 180));

      window->draw(ring);
    } else {
      sf::CircleShape dot(boardData.tileSize / 6.f);
      dot.setPosition({pos.x * boardData.tileSize + boardData.tileSize / 2.f -
                           dot.getRadius(),
                       pos.y * boardData.tileSize + boardData.tileSize / 2.f -
                           dot.getRadius()});

      dot.setFillColor(HIGHTLIGHT_COLOR);

      window->draw(dot);
    }
  }
}

std::string BoardRenderer::getPieceKey(const Piece &p) {
  std::string style =
      boardData.pieceStyle.empty() ? "modern" : boardData.pieceStyle;

  std::string color = (p.color == PieceColor::Black) ? "black_" : "white_";

  switch (p.type) {
  case PieceType::Pawn:
    return style + "/" + color + "pawn";
  case PieceType::Rook:
    return style + "/" + color + "rook";
  case PieceType::Knight:
    return style + "/" + color + "knight";
  case PieceType::Bishop:
    return style + "/" + color + "bishop";
  case PieceType::Queen:
    return style + "/" + color + "queen";
  case PieceType::King:
    return style + "/" + color + "king";
  default:
    return "modern/white_pawn";
  }
}
