#pragma once

#include <string>
#include <map>

#include <GL/glew.h>

static std::map<int, std::string> shaderTypeDic = { {GL_VERTEX_SHADER, "VERTEX"}, {GL_FRAGMENT_SHADER, "FRAGMENT"} };

const std::string parse_shaderType(const int shaderType)
{
	return shaderTypeDic[shaderType];
}
