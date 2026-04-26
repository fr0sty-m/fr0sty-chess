#include "Config.h"

#include <fstream>
#include <iostream>

Config &Config::getInstance() {
  static Config instance;
  return instance;
}

bool Config::load(const std::string &path) {
  filePath = path;
  return reload();
}

bool Config::reload() {
  std::ifstream file(filePath);
  if (!file.is_open())
    return false;

  values.clear();

  std::string line;
  std::string currentSection;

  while (std::getline(file, line)) {

    // comment strip
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos)
      line = line.substr(0, commentPos);

    if (line.empty())
      continue;

    if (line.front() == '[' && line.back() == ']') {
      currentSection = line.substr(1, line.size() - 2);
      continue;
    }

    size_t eq = line.find('=');
    if (eq == std::string::npos)
      continue;

    std::string key = line.substr(0, eq);
    std::string value = line.substr(eq + 1);

    // trim
    key.erase(0, key.find_first_not_of(" "));
    key.erase(key.find_last_not_of(" ") + 1);

    value.erase(0, value.find_first_not_of(" "));
    value.erase(value.find_last_not_of(" ") + 1);

    values[currentSection + "." + key] = value;
  }

  std::cout << "Loading config...\n";
  return true;
}

std::string Config::get(const std::string &key,
                        const std::string &defaultValue) {
  if (values.find(key) == values.end())
    return defaultValue;

  return values[key];
}
