//
// Created by Tyler Sheehan on 4/8/25.
//

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "shader.h"

#include <map>
#include <iostream>

class ShaderManager {
public:
  ShaderManager() = default;

  ~ShaderManager();

  Shader loadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);

  Shader& getShader(std::string name);

  void clear();


private:
  std::map<std::string, Shader> shaders;

  Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile=nullptr);


};



#endif //SHADERMANAGER_H
