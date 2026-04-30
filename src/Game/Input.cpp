#include "Input.h"

#include "Assets/Assets.h"
#include "GameManager.h"
#include "MoveValidator.h"

bool Input::mousePressed = false;
sf::Vector2i Input::dragStartSquare;

void Input::onMousePressed(sf::Vector2i mousePos, BoardData &boardData) {
  mousePressed = true;
  dragStartSquare = {mousePos.x / boardData.tileSize,
                     mousePos.y / boardData.tileSize};
}

void Input::onMouseMoved(sf::Vector2i mousePos, BoardData &boardData) {
  if (!mousePressed)
    return;

  if (!boardData.isDragging &&
      (std::abs(mousePos.x - dragStartSquare.x * boardData.tileSize) >
           DRAG_THRESHOLD ||
       std::abs(mousePos.y - dragStartSquare.y * boardData.tileSize) >
           DRAG_THRESHOLD)) {

    int col = dragStartSquare.x, row = dragStartSquare.y;

    if (col < 0 || col >= 8 || row < 0 || row >= 8 ||
        boardData.grid[row][col].type == PieceType::None)
      return;

    boardData.legalMoves =
        MoveValidator::getLegalMoves({col, row}, boardData.grid);

    boardData.isDragging = true;
    boardData.draggedPiece = boardData.grid[row][col];
    boardData.grid[row][col] = {PieceType::None, PieceColor::None};

    boardData.dragOffset = {(float)mousePos.x - (col * boardData.tileSize +
                                                 boardData.tileSize / 2.f),
                            (float)mousePos.y - (row * boardData.tileSize +
                                                 boardData.tileSize / 2.f)};
  }

  if (boardData.isDragging)
    boardData.currentMousePos = (sf::Vector2f)mousePos;
}

void Input::onMouseReleased(sf::Vector2i mousePos, BoardData &boardData,
                            GameManager *gameManager) {
  if (!mousePressed)
    return;

  int col = mousePos.x / boardData.tileSize;
  int row = mousePos.y / boardData.tileSize;

  if (boardData.isDragging) {
    boardData.grid[dragStartSquare.y][dragStartSquare.x] =
        boardData.draggedPiece;

    if (col >= 0 && col < 8 && row >= 0 && row < 8 &&
        boardData.draggedPiece.color == gameManager->getCurrentTurn() &&
        MoveValidator::isValidMove(dragStartSquare, {col, row},
                                   boardData.grid)) {

      boardData.grid[dragStartSquare.y][dragStartSquare.x] = {PieceType::None,
                                                              PieceColor::None};
      boardData.grid[row][col] = boardData.draggedPiece;

      Assets::getInstance().playSound("piece_move");
      gameManager->addMove(dragStartSquare, {col, row}, boardData.draggedPiece,
                           false);
      gameManager->switchTurn();
    }

    boardData.isDragging = false;
    boardData.selectedSquare = {-1, -1};
    boardData.legalMoves.clear();
  } else {
    // Sürükleme yapılmadıysa tıklama olarak işle
    handleInput(mousePos, boardData, gameManager);
  }

  mousePressed = false;
}

// ================= CLICK MOVE =================

void Input::handleInput(sf::Vector2i mousePos, BoardData &boardData,
                        GameManager *gameManager) {
  int col = mousePos.x / boardData.tileSize,
      row = mousePos.y / boardData.tileSize;

  if (col < 0 || col >= 8 || row < 0 || row >= 8)
    return;

  if (boardData.selectedSquare.x == -1) {
    if (boardData.grid[row][col].type != PieceType::None &&
        boardData.grid[row][col].color == gameManager->getCurrentTurn()) {
      boardData.selectedSquare = {col, row};

      boardData.legalMoves = MoveValidator::getLegalMoves(
          boardData.selectedSquare, boardData.grid);
    }
  } else {

    if (boardData.selectedSquare.x == col &&
        boardData.selectedSquare.y == row) {
      boardData.selectedSquare = {-1, -1};
      boardData.legalMoves.clear();
      return;
    }

    if (MoveValidator::isValidMove(boardData.selectedSquare, {col, row},
                                   boardData.grid)) {

      boardData.movingPiece.piece =
          boardData
              .grid[boardData.selectedSquare.y][boardData.selectedSquare.x];
      boardData.movingPiece.startPos = {
          (float)boardData.selectedSquare.x * boardData.tileSize,
          (float)boardData.selectedSquare.y * boardData.tileSize};
      boardData.movingPiece.endPos = {(float)col * boardData.tileSize,
                                      (float)row * boardData.tileSize};
      boardData.movingPiece.progress = 0.f;
      boardData.movingPiece.active = true;

      boardData.grid[boardData.selectedSquare.y][boardData.selectedSquare.x] = {
          PieceType::None, PieceColor::None};

      bool isCapture = (boardData.grid[row][col].type != PieceType::None);

      Assets::getInstance().playSound("piece_move");
      gameManager->addMove(boardData.selectedSquare, {col, row},
                           boardData.movingPiece.piece, isCapture);
      gameManager->switchTurn();
    }

    boardData.selectedSquare = {-1, -1};
    boardData.legalMoves.clear();
  }
}
