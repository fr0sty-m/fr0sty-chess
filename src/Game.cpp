#include "Game.h"

#include "Assets.h"
#include "Board.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

Game::Game() { this->initGame(); }

void Game::initGame() {
  window = new sf::RenderWindow{sf::VideoMode({800, 800}), "fr0sty Chess v0.1",
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
        // Eğer burada hata basarsa terminale bak, dosya yolu yanlıştır
        printf("ERROR: %s Could not load! Path: %s\n", key.c_str(),
               path.c_str());
      }
    }
  }

  board = new Board(BoardColor::Black);
  board->setupPieces();
}

void Game::events() {
  while (const std::optional event = window->pollEvent()) {
    if (event->is<sf::Event::Closed>())
      window->close();

    if (const auto *mouseEvent =
            event->getIf<sf::Event::MouseButtonPressed>()) {
      if (mouseEvent->button == sf::Mouse::Button::Left) {
        board->handleInput(sf::Mouse::getPosition(*window));
      }
    }
  }
}

void Game::tick() {
  while (window->isOpen()) {
    this->events();

    this->draw();
  }
}

void Game::draw() {
  window->clear();

  board->draw(window);

  window->display();
}
