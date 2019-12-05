#pragma once

#include <string>

#include <glm/vec3.hpp>

#include "OpenGLHelper.h"

bool openObj(const std::string fileName, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &vertexNormals);