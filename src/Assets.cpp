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
