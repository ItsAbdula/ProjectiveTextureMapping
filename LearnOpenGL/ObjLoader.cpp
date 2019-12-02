#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/vec3.hpp>

#include "ObjLoader.h"
#include "OpenGLHelper.h"

bool openObj(const std::string fileName, std::vector<glm::vec3> &vertices)
{
	vertices.clear();

	std::ifstream ifs;
	std::string line;

	char op[3];
	glm::vec3 pos;
	std::vector<glm::vec3> vertexIndices;

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
			// TODO: normal
		}
		else if (strcmp(op, "vt") == false)
		{
			// TODO : texCoord
		}

		if (strncmp(op, "f", 1) == false)
		{
			charPos = 0;
			int vIndex = 0, uvIndex = 0, vnIndex = 0;
			std::vector<int> faceVertexIndicies;

			while ((charPos = line.find(' ')) != std::string::npos)
			{
				sscanf_s(line.substr(0, charPos).c_str(), "%d%*[-/]%d%*[-/]%d", &vIndex, &uvIndex, &vnIndex);
				line.erase(0, charPos + 1);

				faceVertexIndicies.push_back(vIndex - 1);
			}

			for (size_t i = 0; i < faceVertexIndicies.size(); i++)
			{
				if (i > 2) break;

				vertices.push_back(vertexIndices[faceVertexIndicies[i]]);
			}
		}
	}

	ifs.close();

	return true;
}