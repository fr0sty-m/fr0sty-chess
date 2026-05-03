#pragma once

#include <string>
#include <vector>

class ThemeManager {
public:
  void loadThemes(const std::string &basePath);

  std::vector<std::string> getAvailableThemes() const;

private:
  std::vector<std::string> themes;
};
