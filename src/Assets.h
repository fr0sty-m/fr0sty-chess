#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <string>

class Assets {
private:
  std::map<std::string, sf::Texture> textures;
  std::map<std::string, sf::SoundBuffer> soundBuffers;
  std::map<std::string, sf::Font> fonts;
  std::map<std::string, std::unique_ptr<sf::Sound>> readySounds;

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

  bool loadSound(const std::string &name, const std::string &path);
  bool playSound(const std::string &name);
};
