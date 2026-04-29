#include "Functions.h"

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

} // namespace Functions
