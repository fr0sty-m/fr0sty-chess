#include "Colors.h"
#include "Definitions.h"
#include "Piece.h"

namespace Functions {

BoardColor themeToColor(const std::string &theme);

colors selectStyle(BoardColor style);

std::string getPieceKey(const Piece &p);

sf::Vector2f getTileCenter(int col, int row, int tileSize);
} // namespace Functions
