#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <string>

class Assets {
private:
  // Texture'lar
  std::map<std::string, sf::Texture> textures;

  // 🔥 Font'lar
  std::map<std::string, sf::Font> fonts;

  // Singleton
  Assets() = default;

public:
  static Assets &getInstance() {
    static Assets instance;
    return instance;
  }

  // Kopyalamayı engelle
  Assets(const Assets &) = delete;
  void operator=(const Assets &) = delete;

  // TEXTURE
  bool loadTexture(const std::string &name, const std::string &path);
  const sf::Texture &getTexture(const std::string &name) const;

  // 🔥 FONT API
  bool loadFont(const std::string &name, const std::string &path);
  const sf::Font &getFont(const std::string &name) const;
};
