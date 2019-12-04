#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/vec3.hpp>

#include "ObjLoader.h"
#include "OpenGLHelper.h"
#include "Logger.h"

bool openObj(const std::string fileName, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &vertexNormals)
{
	vertices.clear();
	vertexNormals.clear();

	std::ifstream ifs;
	std::string line;

	char op[3];
	glm::vec3 pos;
	std::vector<glm::vec3> vertexIndices;
	std::vector<glm::vec3> vertexNormalIndices;

	ifs.open("../Models/" + fileName);

	int charPos = 0;
	while (std::getline(ifs, line))
	{
		if (line[0] == '\n' || line[0] == '#' || line[0] == '!' || line[0] == '$') continue;

		sscanf_s(line.c_str(), "%s", op, sizeof(op));

		pos.x = 0; pos.y = 0; pos.z = 0;

		charPos = 0;
		if ((charPos = line.find(' ')) != std::string::npos)
		{
			line.erase(0, charPos + 1);
		}

		if (strcmp(op, "v") == false)
		{
			sscanf_s(line.c_str(), "%f %f %f", &pos.x, &pos.y, &pos.z);
			vertexIndices.push_back(pos);
		}
		else if (strcmp(op, "vn") == false)
		{
			sscanf_s(line.c_str(), "%f %f %f", &pos.x, &pos.y, &pos.z);
			vertexNormalIndices.push_back(pos);
		}
		else if (strcmp(op, "vt") == false)
		{
			// TODO : texCoord
		}

		if (strncmp(op, "f", 1) == false)
		{
			int vIndex = 0, uvIndex = 0, vnIndex = 0;
			std::vector<int> faceVertexIndicies;
			std::vector<int> faceVertexNormalIndicies;

			charPos = 0;
			while ((charPos = line.find(' ')) != std::string::npos)
			{
				sscanf_s(line.substr(0, charPos).c_str(), "%d%*[-/]%d%*[-/]%d", &vIndex, &uvIndex, &vnIndex);
				line.erase(0, charPos + 1);

				faceVertexIndicies.push_back(vIndex - 1);
				faceVertexNormalIndicies.push_back(vnIndex - 1);
			}

			if (faceVertexIndicies.size() == 3)
			{
				vertices.push_back(vertexIndices[faceVertexIndicies[0]]);
				vertices.push_back(vertexIndices[faceVertexIndicies[1]]);
				vertices.push_back(vertexIndices[faceVertexIndicies[2]]);
			}
			else if (faceVertexIndicies.size() == 4)
			{
				vertices.push_back(vertexIndices[faceVertexIndicies[0]]);
				vertices.push_back(vertexIndices[faceVertexIndicies[1]]);
				vertices.push_back(vertexIndices[faceVertexIndicies[2]]);

				vertices.push_back(vertexIndices[faceVertexIndicies[0]]);
				vertices.push_back(vertexIndices[faceVertexIndicies[2]]);
				vertices.push_back(vertexIndices[faceVertexIndicies[3]]);
			}
			else
			{
				GLchar infoLog[512] = { 0, };
				log_warn(infoLog, "faceVertexIndices.size() : " + std::to_string(faceVertexIndicies.size()));
			}

			if (faceVertexNormalIndicies.size() == 3)
			{
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[0]]);
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[1]]);
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[2]]);
			}
			else if (faceVertexNormalIndicies.size() == 4)
			{
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[0]]);
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[1]]);
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[2]]);

				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[0]]);
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[2]]);
				vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[3]]);
			}
			else
			{
				GLchar infoLog[512] = { 0, };
				log_warn(infoLog, "faceVertexNormalIndices.size() : " + std::to_string(faceVertexNormalIndicies.size()));
			}
		}
	}

	ifs.close();

	return true;
}