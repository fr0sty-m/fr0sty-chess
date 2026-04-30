#include "Functions.h"

#include "BoardData.h"
#include "Definitions.h"

namespace Functions {

BoardColor themeToColor(const std::string &theme) {
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

colors selectStyle(BoardColor style) {
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

std::string getPieceKey(const Piece &p, const BoardData &boardData) {
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

sf::Vector2f getTileCenter(int col, int row, int tileSize) {
  return sf::Vector2f(col * tileSize + tileSize / 2.f,
                      row * tileSize + tileSize / 2.f);
}

} // namespace Functions
