#include "Game.h"

#include "Assets.h"
#include "Board.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

Game::Game() { this->initGame(); }

void Game::initGame() {
  window = new sf::RenderWindow{sf::VideoMode({800, 800}), "fr0sty Chess",
                                sf::Style::Close};
  window->setFramerateLimit(120);

  auto &assets = Assets::getInstance();

  // Yükleme listesi (İsimleri Board.cpp'deki switch ile uyumlu yapıyoruz)
  std::vector<std::string> colors = {"white_", "black_"};
  std::vector<std::string> types = {"pawn",   "rook",  "knight",
                                    "bishop", "queen", "king"};

  for (const auto &c : colors) {
    for (const auto &t : types) {
      std::string key = c + t;
      std::string path = "assets/" + key + ".png";

      if (!assets.loadTexture(key, path)) {
        printf("ERROR: %s Could not load! Path: %s\n", key.c_str(),
               path.c_str());
      }
    }
  }

  board = new Board(BoardColor::Blue);
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
