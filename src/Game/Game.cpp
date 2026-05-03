#include "Game.h"

#include "Assets/Assets.h"
#include "Board/Board.h"
#include "Config/Config.h"

#include "Utils/Functions.h"
#include "Utils/Piece.h"

Game::Game() { this->initGame(); }

void Game::initGame() {
  window = new sf::RenderWindow{sf::VideoMode({1280, 800}), "fr0sty Chess",
                                sf::Style::Close};
  window->setFramerateLimit(120);

  auto &assets = Assets::getInstance();
  auto &cfg = Config::getInstance();

  themeManager.loadThemes("assets/theme");

  cfg.load("./config.ini");

  std::string boardTheme = cfg.get("theme.board", "teal");
  std::string pieceTheme = cfg.get("theme.pieces", "modern");

  board = new Board(boardTheme, pieceTheme, &gm);
  board->setupPieces();
}

void Game::events() {
  while (const std::optional event = window->pollEvent()) {

    if (event->is<sf::Event::Closed>())
      window->close();

    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

    // Pressed
    if (const auto *mouseEvent =
            event->getIf<sf::Event::MouseButtonPressed>()) {
      if (mouseEvent->button == sf::Mouse::Button::Left) {
        board->handleMousePressed(mousePos);
      }
    }

    // Released
    if (const auto *mouseEvent =
            event->getIf<sf::Event::MouseButtonReleased>()) {
      if (mouseEvent->button == sf::Mouse::Button::Left) {
        board->handleMouseReleased(mousePos);
      }
    }

    board->handleMouseMoved(mousePos);

    if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>()) {
      // RELOAD CONFIG ON PRESS 'F5'
      if (keyEvent->code == sf::Keyboard::Key::F5) {
        Config::getInstance().reload();

        std::string newTheme = Config::getInstance().get("theme.board", "teal");
        std::string newPieces =
            Config::getInstance().get("theme.pieces", "modern");

        board->setTheme(Functions::themeToColor(newTheme));
        board->setPieceStyle(newPieces);
      }
      // RESTART GAME ON PRESS 'R'
      if (keyEvent->code == sf::Keyboard::Key::R) {
        board =
            new Board(Config::getInstance().get("theme.board", "teal"),
                      Config::getInstance().get("theme.pieces", "modern"), &gm);
        board->setupPieces();
        gm.setCurrentTurn(PieceColor::White);
      }
    }
  }
}

void Game::tick() {
  sf::Clock clock;

  while (window->isOpen()) {
    float dt = clock.restart().asSeconds();

    this->events();

    board->update(dt);

    this->draw();
  }
}

void Game::draw() {
  window->clear();

  board->draw(window);

  window->display();
}
