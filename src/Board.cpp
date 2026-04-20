#include "Board.h"

#include "Assets.h"

#include <iostream>

Board::Board(BoardColor boardColor)
    : boardColors(selectStyle(boardColor)), tileSize(100) {
  selectedSquare = {-1, -1};
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

void Board::handleInput(sf::Vector2i mousePos) {
  int col = mousePos.x / tileSize;
  int row = mousePos.y / tileSize;

  // Sınır kontrolü
  if (col < 0 || col >= 8 || row < 0 || row >= 8)
    return;

  if (selectedSquare.x == -1) {
    // Taş seçme aşaması
    if (grid[row][col].type != PieceType::None) {
      selectedSquare = {col, row};
    }
  } else {
    // Taşıma aşaması
    // Kaynak karedeki taşı hedef kareye kopyala
    grid[row][col] = grid[selectedSquare.y][selectedSquare.x];

    // Eski kareyi boşalt
    grid[selectedSquare.y][selectedSquare.x] = {PieceType::None,
                                                PieceColor::None};

    // Seçimi temizle
    selectedSquare = {-1, -1};
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
      if (selectedSquare.x == col && selectedSquare.y == row) {
        sf::RectangleShape highlight(
            sf::Vector2f({(float)tileSize, (float)tileSize}));
        highlight.setPosition({(float)col * tileSize, (float)row * tileSize});

        // Hafif sarı ve yarı saydam (Alpha: 150)
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
}
