#include "Game.h"

#include "Assets.h"
#include "Board.h"
#include "Config.h"

#include <iostream>

Game::Game() { this->initGame(); }

void Game::initGame() {
  window = new sf::RenderWindow{sf::VideoMode({1280, 800}), "fr0sty Chess",
                                sf::Style::Close};
  window->setFramerateLimit(120);

  auto &assets = Assets::getInstance();
  auto &cfg = Config::getInstance();

  cfg.load("./config.ini");

  std::string boardTheme = cfg.get("theme.board", "teal");
  std::string pieceTheme = cfg.get("theme.pieces", "modern");

  std::vector<std::string> styles = {"modern", "cute"};
  std::vector<std::string> colors = {"white_", "black_"};
  std::vector<std::string> types = {"pawn",   "rook",  "knight",
                                    "bishop", "queen", "king"};

  for (const auto &style : styles) {
    for (const auto &c : colors) {
      for (const auto &t : types) {

        std::string key = style + "/" + c + t;
        std::string path = "assets/" + style + "/" + c + t + ".png";

        assets.loadTexture(key, path);
      }
    }
  }

  board = new Board(boardTheme, pieceTheme, &gm);
  board->setupPieces();
}

void Game::events() {
  while (const std::optional event = window->pollEvent()) {

    if (event->is<sf::Event::Closed>())
      window->close();

    if (const auto *mouseEvent =
            event->getIf<sf::Event::MouseButtonPressed>()) {

      if (mouseEvent->button == sf::Mouse::Button::Left) {
        board->onMousePressed(sf::Mouse::getPosition(*window));
      }
    }

    if (const auto *mouseEvent =
            event->getIf<sf::Event::MouseButtonReleased>()) {

      if (mouseEvent->button == sf::Mouse::Button::Left) {
        board->onMouseReleased(sf::Mouse::getPosition(*window));
      }
    }

    if (const auto *mouseEvent = event->getIf<sf::Event::MouseMoved>()) {
      board->onMouseMoved(sf::Mouse::getPosition(*window));
    }

    if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>()) {
      if (keyEvent->code == sf::Keyboard::Key::F5) {
        Config::getInstance().reload();

        std::string newTheme = Config::getInstance().get("theme.board", "teal");
        std::string newPieces =
            Config::getInstance().get("theme.pieces", "modern");

        board->setTheme(Board::themeToColor(newTheme));
        board->setPieceStyle(newPieces);
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
