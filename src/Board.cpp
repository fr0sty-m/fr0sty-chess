#include "Board.h"

#include "Assets.h"
#include "MoveValidator.h"
#include "Piece.h"

#include <iostream>

Board::Board(BoardColor boardColor)
    : boardColors(selectStyle(boardColor)), tileSize(100) {
  selectedSquare = {-1, -1};
  movingPiece.active = false;
  isDragging = false;
}

colors Board::selectStyle(BoardColor boardStyle) {
  sf::Color darkColor;
  sf::Color lightColor;

  switch (boardStyle) {
  case BoardColor::Green:
    lightColor = {238, 238, 210};
    darkColor = {118, 150, 86};
    break;
  case BoardColor::Red:
    lightColor = {238, 238, 210};
    darkColor = {227, 93, 93};
    break;
  case BoardColor::Blue:
    lightColor = {238, 238, 210};
    darkColor = {97, 116, 201};
    break;
  case BoardColor::Black:
    lightColor = {238, 238, 210};
    darkColor = {53, 53, 53};
    break;
  default:
    lightColor = {238, 238, 210};
    darkColor = {118, 150, 86};
    break;
  }

  return {darkColor, lightColor};
}

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
    // 1. Taşı geçici olarak yerine koy (Validator'ın görmesi için)
    grid[dragStartSquare.y][dragStartSquare.x] = draggedPiece;

    // 2. Şimdi kontrol et
    if (col >= 0 && col < 8 && row >= 0 && row < 8 &&
        MoveValidator::isValidMove(dragStartSquare, {col, row}, grid)) {

      // Hamle geçerliyse: Eski yeri boşalt, yeni yere taşı
      grid[dragStartSquare.y][dragStartSquare.x] = {PieceType::None,
                                                    PieceColor::None};
      grid[row][col] = draggedPiece;
    } else {
      // Hamle geçersizse: Taş zaten yukarıda geri konuldu, ekstra bir şey
      // yapmaya gerek yok.
    }

    isDragging = false;
    selectedSquare = {-1, -1};
  } else {
    handleInput(mousePos);
  }
  mousePressed = false;
}

void Board::handleInput(sf::Vector2i mousePos) {
  int col = mousePos.x / tileSize, row = mousePos.y / tileSize;
  if (col < 0 || col >= 8 || row < 0 || row >= 8)
    return;

  if (selectedSquare.x == -1) {
    if (grid[row][col].type != PieceType::None)
      selectedSquare = {col, row};
  } else {
    if (selectedSquare.x == col && selectedSquare.y == row) {
      selectedSquare = {-1, -1};
      return;
    }

    // VALIDATOR BURADA DA DEVREYE GİRİYOR
    if (MoveValidator::isValidMove(selectedSquare, {col, row}, grid)) {
      movingPiece.piece = grid[selectedSquare.y][selectedSquare.x];
      movingPiece.startPos = {(float)selectedSquare.x * tileSize,
                              (float)selectedSquare.y * tileSize};
      movingPiece.endPos = {(float)col * tileSize, (float)row * tileSize};
      movingPiece.progress = 0.f;
      movingPiece.active = true;
      grid[selectedSquare.y][selectedSquare.x] = {PieceType::None,
                                                  PieceColor::None};
    }
    selectedSquare = {-1, -1};
  }
}

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

void Board::setupPieces() {
  // Arka sıra taşlarını dizelim (Rooks, Knights,
  // Bishops, Queen, King)
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

void Board::draw(sf::RenderWindow *window) {
  auto &assets = Assets::getInstance();

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // 1. KAREYİ ÇİZ (Senin mevcut kodun)
      sf::RectangleShape square(
          sf::Vector2f({(float)tileSize, (float)tileSize}));
      square.setPosition({(float)col * tileSize, (float)row * tileSize});
      square.setFillColor(((row + col) % 2 == 0) ? boardColors.white
                                                 : boardColors.black);
      window->draw(square);

      // --- SEÇİLİ KAREYİ BOYAMA BURASI ---
      if (!isDragging && selectedSquare.x == col && selectedSquare.y == row &&
          grid[row][col].type != PieceType::None) {
        sf::RectangleShape highlight(
            sf::Vector2f({(float)tileSize, (float)tileSize}));

        highlight.setPosition({(float)col * tileSize, (float)row * tileSize});
        highlight.setFillColor(sf::Color(209, 71, 71, 150));
        window->draw(highlight);
      }
      // -----------------------------------

      // 2. TAŞI ÇİZ
      Piece &p = grid[row][col];
      if (p.type != PieceType::None) {
        std::string colorStr =
            (p.color == PieceColor::Black) ? "black_" : "white_";
        std::string typeStr;

        switch (p.type) {
        case PieceType::Pawn:
          typeStr = "pawn";
          break;
        case PieceType::Rook:
          typeStr = "rook";
          break;
        case PieceType::Knight:
          typeStr = "knight";
          break;
        case PieceType::Bishop:
          typeStr = "bishop";
          break;
        case PieceType::Queen:
          typeStr = "queen";
          break;
        case PieceType::King:
          typeStr = "king";
          break;
        default:
          break;
        }

        if (!typeStr.empty()) {
          const sf::Texture &tex = assets.getTexture(colorStr + typeStr);
          sf::Sprite pieceSprite(tex);

          // --- ÖLÇEKLENDİRME BURADA BAŞLIYOR ---
          // 100.0f / 512.0f = ~0.195 (Yani resmi %19'una indiriyoruz)
          float scale = (float)tileSize / tex.getSize().x;
          pieceSprite.setScale({scale, scale});
          // -------------------------------------

          pieceSprite.setPosition(
              {(float)col * tileSize, (float)row * tileSize});
          window->draw(pieceSprite);
        }
      }
    }
  }

  if (movingPiece.active) {
    auto &assets = Assets::getInstance();

    std::string colorStr =
        (movingPiece.piece.color == PieceColor::Black) ? "black_" : "white_";

    std::string typeStr;

    switch (movingPiece.piece.type) {
    case PieceType::Pawn:
      typeStr = "pawn";
      break;
    case PieceType::Rook:
      typeStr = "rook";
      break;
    case PieceType::Knight:
      typeStr = "knight";
      break;
    case PieceType::Bishop:
      typeStr = "bishop";
      break;
    case PieceType::Queen:
      typeStr = "queen";
      break;
    case PieceType::King:
      typeStr = "king";
      break;
    default:
      break;
    }

    if (!typeStr.empty()) {
      const sf::Texture &tex = assets.getTexture(colorStr + typeStr);
      sf::Sprite sprite(tex);

      float scale = (float)tileSize / tex.getSize().x;
      sprite.setScale({scale, scale});

      // 🔥 smooth movement (lerp)
      sf::Vector2f pos =
          movingPiece.startPos +
          (movingPiece.endPos - movingPiece.startPos) * movingPiece.progress;

      sprite.setPosition(pos);
      window->draw(sprite);
    }
  }

  if (isDragging) {
    auto &assets = Assets::getInstance();

    std::string colorStr =
        (draggedPiece.color == PieceColor::Black) ? "black_" : "white_";

    std::string typeStr;

    switch (draggedPiece.type) {
    case PieceType::Pawn:
      typeStr = "pawn";
      break;
    case PieceType::Rook:
      typeStr = "rook";
      break;
    case PieceType::Knight:
      typeStr = "knight";
      break;
    case PieceType::Bishop:
      typeStr = "bishop";
      break;
    case PieceType::Queen:
      typeStr = "queen";
      break;
    case PieceType::King:
      typeStr = "king";
      break;
    default:
      break;
    }

    if (!typeStr.empty()) {
      const sf::Texture &tex = assets.getTexture(colorStr + typeStr);
      sf::Sprite sprite(tex);

      float scale = (float)tileSize / tex.getSize().x;
      sprite.setScale({scale, scale});

      sprite.setPosition(currentMousePos - dragOffset);

      window->draw(sprite);
    }
  }
}
