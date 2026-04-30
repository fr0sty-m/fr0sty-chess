#include "Board.h"

#include "Assets/Assets.h"
#include "Game/Input.h"
#include "Utils/BoardData.h"
#include "Utils/Functions.h"

Board::Board(std::string boardColor, std::string pieceStyle, GameManager *gm)
    : gameManager(gm),
      boardData(100, pieceStyle,
                Functions::selectStyle(Functions::themeToColor(boardColor))) {

  renderer = new BoardRenderer(gameManager, boardData);

  // Asset yüklemeleri
  Assets::getInstance().loadFont("coord_font", "assets/fonts/Hack-Bold.ttf");
  Assets::getInstance().loadSound("piece_move",
                                  "assets/sounds/chess_piece.wav");
}

Board::~Board() { delete renderer; }

void Board::update(float dt) {
  // Animasyon mantığı artık BoardData içindeki movingPiece üzerinden yürüyor
  if (!boardData.movingPiece.active)
    return;

  boardData.movingPiece.progress += dt * 8.f;

  if (boardData.movingPiece.progress >= 1.f) {
    boardData.movingPiece.progress = 1.f;
    boardData.movingPiece.active = false;

    int col = boardData.movingPiece.endPos.x / boardData.tileSize;
    int row = boardData.movingPiece.endPos.y / boardData.tileSize;

    boardData.grid[row][col] = boardData.movingPiece.piece;
  }
}

void Board::handleMousePressed(sf::Vector2i mousePos) {
  Input::onMousePressed(mousePos, boardData);
}

void Board::handleMouseMoved(sf::Vector2i mousePos) {
  Input::onMouseMoved(mousePos, boardData);
}

void Board::handleMouseReleased(sf::Vector2i mousePos) {
  Input::onMouseReleased(mousePos, boardData, gameManager);
}

void Board::draw(sf::RenderWindow *window) {
  // Renderer zaten constructor'da aldığın 'data' referansına sahip
  renderer->draw(window);
}

void Board::setupPieces() {
  PieceType backRow[] = {
      PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
      PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook};

  for (int i = 0; i < 8; i++) {
    boardData.grid[0][i] = {backRow[i], PieceColor::Black};
    boardData.grid[1][i] = {PieceType::Pawn, PieceColor::Black};
    boardData.grid[7][i] = {backRow[i], PieceColor::White};
    boardData.grid[6][i] = {PieceType::Pawn, PieceColor::White};
  }
}

void Board::setTheme(BoardColor newColor) {
  boardData.boardColors = Functions::selectStyle(newColor);
}

void Board::setPieceStyle(const std::string &style) {
  boardData.pieceStyle = style.empty() ? "modern" : style;
}
