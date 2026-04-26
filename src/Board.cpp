#include "Board.h"

#include "Assets.h"
#include "MoveValidator.h"

#include "Config.h"
#include "Definitions.h"

#include <cmath>

// ================= CONSTRUCTOR =================

Board::Board(std::string boardColor, std::string pieceStyle, GameManager *gm)
    : boardColors(selectStyle(themeToColor(boardColor))), tileSize(100),
      pieceStyle(pieceStyle), gameManager(gm) {
  selectedSquare = {-1, -1};

  Assets::getInstance().loadFont("main",
                                 "assets/fonts/CaskaydiaCoveNerdFont-Bold.ttf");

  Assets::getInstance().loadFont("coord_font", "assets/fonts/Hack-Bold.ttf");

  Assets::getInstance().loadSound("piece_move",
                                  "assets/sounds/chess_piece.wav");

  std::string theme = Config::getInstance().get("theme.board", "teal");
}

// ================= STYLE =================

BoardColor Board::themeToColor(const std::string &theme) {
  if (theme == "green")
    return BoardColor::Green;
  if (theme == "red")
    return BoardColor::Red;
  if (theme == "blue")
    return BoardColor::Blue;
  if (theme == "orange")
    return BoardColor::Orange;
  if (theme == "black")
    return BoardColor::Black;
  if (theme == "dark")
    return BoardColor::Dark;
  if (theme == "purple")
    return BoardColor::Purple;
  if (theme == "teal")
    return BoardColor::Teal;
  if (theme == "gray")
    return BoardColor::Gray;
  if (theme == "chesscom")
    return BoardColor::Chesscom;
  if (theme == "lichess")
    return BoardColor::Lichess;

  return BoardColor::Teal;
}

colors Board::selectStyle(BoardColor style) {
  switch (style) {

  case BoardColor::Green:
    return {GREEN_DARK, GREEN_LIGHT};

  case BoardColor::Red:
    return {RED_DARK, RED_LIGHT};

  case BoardColor::Blue:
    return {BLUE_DARK, BLUE_LIGHT};

  case BoardColor::Orange:
    return {ORANGE_DARK, ORANGE_LIGHT};

  case BoardColor::Black:
    return {BLACK_DARK, BLACK_LIGHT};

  case BoardColor::Dark:
    return {DARK_DARK, DARK_LIGHT};

  case BoardColor::Purple:
    return {PURPLE_DARK, PURPLE_LIGHT};

  case BoardColor::Teal:
    return {TEAL_DARK, TEAL_LIGHT};

  case BoardColor::Gray:
    return {GRAY_DARK, GRAY_LIGHT};

  case BoardColor::Chesscom:
    return {CHESS_DARK, CHESS_LIGHT};

  case BoardColor::Lichess:
    return {LICHESS_DARK, LICHESS_LIGHT};

  default:
    return {TEAL_DARK, TEAL_LIGHT};
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

    legalMoves = MoveValidator::getLegalMoves({col, row}, grid);

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

      Assets::getInstance().playSound("piece_move");

      gameManager->addMove(dragStartSquare, {col, row}, draggedPiece, false);
      gameManager->switchTurn();
      // legalMoves.clear();
    }

    isDragging = false;
    selectedSquare = {-1, -1};
    legalMoves.clear();
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

      legalMoves = MoveValidator::getLegalMoves(selectedSquare, grid);
    }
  } else {

    if (selectedSquare.x == col && selectedSquare.y == row) {
      selectedSquare = {-1, -1};
      legalMoves.clear();
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

      Assets::getInstance().playSound("piece_move");
      gameManager->addMove(selectedSquare, {col, row}, movingPiece.piece,
                           isCapture);
      gameManager->switchTurn();
    }

    selectedSquare = {-1, -1};
    legalMoves.clear();
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
  std::string style = pieceStyle.empty() ? "modern" : pieceStyle;

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

void Board::drawHighlight(sf::RenderWindow *window) {
  if (selectedSquare.x < 0 || selectedSquare.y < 0)
    return;

  if (isDragging)
    return;

  if (grid[selectedSquare.y][selectedSquare.x].type == PieceType::None)
    return;

  sf::RectangleShape highlight(sf::Vector2f((float)tileSize, (float)tileSize));

  highlight.setPosition(
      {selectedSquare.x * (float)tileSize, selectedSquare.y * (float)tileSize});

  highlight.setFillColor(HIGHTLIGHT_COLOR);

  window->draw(highlight);
}

void Board::drawUI(sf::RenderWindow *window, const sf::Font &font) {
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
    sf::Color tileColor = light ? boardColors.white : boardColors.black;

    int luminance =
        0.299f * tileColor.r + 0.587f * tileColor.g + 0.114f * tileColor.b;

    t.setFillColor(luminance > 128 ? sf::Color::Black : sf::Color::White);

    t.setPosition({col * tileSize + tileSize - 18.f, 8 * tileSize - 20.f});
    window->draw(t);
  }

  for (int row = 0; row < 8; row++) {
    sf::Text t(font);
    t.setString(std::string(1, '8' - row));
    t.setCharacterSize(16);

    bool light = ((row + 0) % 2 == 0);
    sf::Color tileColor = light ? boardColors.white : boardColors.black;

    int luminance =
        0.299f * tileColor.r + 0.587f * tileColor.g + 0.114f * tileColor.b;

    t.setFillColor(luminance > 128 ? sf::Color::Black : sf::Color::White);

    t.setPosition({4.f, row * tileSize + 4.f});
    window->draw(t);
  }
}

void Board::drawLegalMoves(sf::RenderWindow *window) {
  for (auto &pos : legalMoves) {

    bool isCapture = grid[pos.y][pos.x].type != PieceType::None;

    if (isCapture) {
      sf::CircleShape ring(tileSize / 2.f - 6.f);
      ring.setPosition({pos.x * tileSize + 6.f, pos.y * tileSize + 6.f});

      ring.setFillColor(sf::Color::Transparent);
      ring.setOutlineThickness(4.f);
      ring.setOutlineColor(sf::Color(120, 255, 120, 180));

      window->draw(ring);
    } else {
      sf::CircleShape dot(tileSize / 6.f);
      dot.setPosition({pos.x * tileSize + tileSize / 2.f - dot.getRadius(),
                       pos.y * tileSize + tileSize / 2.f - dot.getRadius()});

      dot.setFillColor(HIGHTLIGHT_COLOR);

      window->draw(dot);
    }
  }
}

// ================= DRAW =================

void Board::draw(sf::RenderWindow *window) {
  auto &assets = Assets::getInstance();
  const sf::Font &coordFont = assets.getFont("coord_font");

  // 1. TILE + HIGHLIGHT + PIECES (layer together)
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {

      drawTile(window, col, row);

      if (!isDragging && selectedSquare.x == col && selectedSquare.y == row &&
          grid[row][col].type != PieceType::None) {

        sf::RectangleShape highlight(
            sf::Vector2f((float)tileSize, (float)tileSize));

        highlight.setPosition({col * (float)tileSize, row * (float)tileSize});

        highlight.setFillColor(HIGHTLIGHT_COLOR);

        window->draw(highlight);
      }

      drawLegalMoves(window);
      drawPiece(window, grid[row][col], col, row);
    }
  }

  // 2. overlays
  drawMoving(window);

  // 3. UI + coords
  drawCoords(window, coordFont);
  drawUI(window, coordFont);
  drawDragged(window);
}

void Board::setTheme(BoardColor newColor) {
  boardColors = selectStyle(newColor);
}

void Board::setPieceStyle(const std::string &style) {
  pieceStyle = style.empty() ? "modern" : style;
}
