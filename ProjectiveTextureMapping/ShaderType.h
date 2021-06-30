#pragma once

#include <string>
#include <map>

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31

static std::map<int, std::string> shaderTypeDic = { {GL_VERTEX_SHADER, "VERTEX"}, {GL_FRAGMENT_SHADER, "FRAGMENT"} };

const std::string parse_shaderType(const int shaderType)
{
	return shaderTypeDic[shaderType];
}
