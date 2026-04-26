#include "Assets.h"

bool Assets::loadTexture(const std::string &name, const std::string &path) {
  sf::Texture tex;
  if (!tex.loadFromFile(path)) {
    return false;
  }
  tex.setSmooth(true);
  textures[name] = std::move(tex);
  return true;
}

const sf::Texture &Assets::getTexture(const std::string &name) const {
  return textures.at(name);
}

bool Assets::loadFont(const std::string &name, const std::string &path) {
  sf::Font font;
  if (!font.openFromFile(path)) {
    return false;
  }
  fonts[name] = std::move(font);
  return true;
}

const sf::Font &Assets::getFont(const std::string &name) const {
  return fonts.at(name);
}
