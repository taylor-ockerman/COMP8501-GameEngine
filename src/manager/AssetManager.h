//
// Created by taylo on 2/18/2026.
//

#ifndef INC_8051TUTORIAL_ASSETMANAGER_H
#define INC_8051TUTORIAL_ASSETMANAGER_H
#include <unordered_map>

#include "../ecs/Component.h"

class AssetManager {
  static std::unordered_map<std::string, Animation> animations;
  static std::unordered_map<std::string, TTF_Font *> fonts;

  static Animation loadAnimationFromXML(const char *path);

public:
  static void loadAnimation(const std::string &clipName, const char *path);

  static void loadFont(const std::string &name, const char *path, float fontSize);

  static TTF_Font *getFont(const std::string &name);

  static const Animation &getAnimation(const std::string &clipName);
};

#endif //INC_8051TUTORIAL_ASSETMANAGER_H
