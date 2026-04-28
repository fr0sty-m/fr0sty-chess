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

bool Assets::loadSound(const std::string &name, const std::string &path) {
  // 1. Önce buffer'ı yükle
  if (!soundBuffers[name].loadFromFile(path))
    return false;

  readySounds[name] = std::make_unique<sf::Sound>(soundBuffers[name]);

  return true;
}

bool Assets::playSound(const std::string &name) {
  auto it = readySounds.find(name);

  if (it != readySounds.end()) {
    // it->second bir unique_ptr<sf::Sound>
    it->second->stop();
    it->second->play();
    return true;
  }

  return false;
}
