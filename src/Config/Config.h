#pragma once

#include <string>
#include <unordered_map>

class Config {
private:
  std::unordered_map<std::string, std::string> values;
  std::string filePath;

  Config() = default;

public:
  static Config &getInstance();

  bool load(const std::string &path);
  bool reload();

  std::string get(const std::string &key, const std::string &defaultValue = "");
};
