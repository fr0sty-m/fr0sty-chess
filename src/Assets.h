#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <string>

class Assets {
private:
  // Texture'ları isimleriyle tutan harita
  std::map<std::string, sf::Texture> textures;

  // Singleton yapısı (Her yerden Assets::getInstance() ile erişmek için)
  Assets() = default;

public:
  static Assets &getInstance() {
    static Assets instance;
    return instance;
  }

  // Kopyalamayı engelle (Singleton kuralı)
  Assets(const Assets &) = delete;
  void operator=(const Assets &) = delete;

  // Texture yükleme fonksiyonu
  bool loadTexture(const std::string &name, const std::string &path);

  // Texture'a erişim
  const sf::Texture &getTexture(const std::string &name) const;
};
