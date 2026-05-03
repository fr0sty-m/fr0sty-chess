#include "ThemeManager.h"

#include "Assets/Assets.h"

#include <filesystem>
#include <iostream>

void ThemeManager::loadThemes(const std::string &basePath) {
  auto &assets = Assets::getInstance();

  themes.clear();

  for (const auto &dir : std::filesystem::directory_iterator(basePath)) {
    if (!dir.is_directory())
      continue;

    std::string style = dir.path().filename().string();
    themes.push_back(style);

    for (const auto &file : std::filesystem::directory_iterator(dir.path())) {
      if (!file.is_regular_file())
        continue;

      if (file.path().extension() != ".png")
        continue;

      std::string name = file.path().stem().string();

      std::string key = style + "/" + name;
      std::string path = file.path().string();

      assets.loadTexture(key, path);

      std::cout << "[Theme] Loaded: " << key << "\n";
    }
  }
}

std::vector<std::string> ThemeManager::getAvailableThemes() const {
  return themes;
}
