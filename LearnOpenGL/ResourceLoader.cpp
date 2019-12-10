#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ResourceLoader.h"

Image::Image(int _width, int _height, int _cahnnel, unsigned char *_data)
{
	width = _width;
	height = _height;
	channel = _cahnnel;
	data = _data;
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

unsigned char *Image::getData()
{
	return data;
}

std::string get_extension(const std::string &filePath)
{
	return filePath.substr(filePath.find_last_of(".") + 1);
}

bool openObj(const std::string fileName, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &vertexTexCoord, std::vector<glm::vec3> &vertexNormals)
{
	vertices.clear();
	vertexTexCoord.clear();
	vertexNormals.clear();

	std::ifstream ifs;
	std::string line;

	char op[3];
	std::vector<glm::vec3> vertexIndices;
	std::vector<glm::vec2> vertexTexCoordIndices;
	std::vector<glm::vec3> vertexNormalIndices;

	ifs.open("../Models/" + fileName);

	int charPos = 0;
	while (std::getline(ifs, line))
	{
		if (line[0] == NULL || line[0] == '\n' || line[0] == '#' || line[0] == '!' || line[0] == '$' || line[0] == 'o' || line[0] == 'm' || line[0] == 'u') continue;

		sscanf_s(line.c_str(), "%s", op, sizeof(op));

		charPos = 0;
		if ((charPos = line.find(' ')) != std::string::npos)
		{
			line.erase(0, charPos + 1);
		}

		if (strcmp(op, "v") == false)
		{
			glm::vec3 pos = { 0,0,0 };
			sscanf_s(line.c_str(), "%f %f %f", &pos.x, &pos.y, &pos.z);
			vertexIndices.push_back(pos);
		}
		else if (strcmp(op, "vn") == false)
		{
			glm::vec3 pos = { 0,0,0 };
			sscanf_s(line.c_str(), "%f %f %f", &pos.x, &pos.y, &pos.z);
			vertexNormalIndices.push_back(pos);
		}
		else if (strcmp(op, "vt") == false)
		{
			glm::vec2 pos = { 0,0 };
			sscanf_s(line.c_str(), "%f %f", &pos.x, &pos.y);
			vertexTexCoordIndices.push_back(pos);
		}

		if (strncmp(op, "f", 1) == false)
		{
			int vIndex = 0, uvIndex = 0, vnIndex = 0;
			std::vector<int> faceVertexIndicies;
			std::vector<int> faceVertexTexCoordIndicies;
			std::vector<int> faceVertexNormalIndicies;

			charPos = 0;
			while ((charPos = line.find(' ')) != std::string::npos)
			{
				if (line.find("//") == std::string::npos)
				{
					sscanf_s(line.substr(0, charPos).c_str(), "%d%*[-/]%d%*[-/]%d", &vIndex, &uvIndex, &vnIndex);
					line.erase(0, charPos + 1);
				}
				else
				{
					uvIndex = 0;

					sscanf_s(line.substr(0, charPos).c_str(), "%d%*[-//]%d", &vIndex, &vnIndex);
					line.erase(0, charPos + 1);
				}

				if (vIndex >= 1)
				{
					faceVertexIndicies.push_back(vIndex - 1);
				}
				if (uvIndex >= 1)
				{
					faceVertexTexCoordIndicies.push_back(uvIndex - 1);
				}
				if (vnIndex >= 1)
				{
					faceVertexNormalIndicies.push_back(vnIndex - 1);
				}
			}

			if (line.size() > 0)
			{
				if (line.find("//") == std::string::npos)
				{
					sscanf_s(line.substr(0, charPos).c_str(), "%d%*[-/]%d%*[-/]%d", &vIndex, &uvIndex, &vnIndex);
					line.erase(0, charPos + 1);
				}
				else
				{
					uvIndex = 0;

					sscanf_s(line.substr(0, charPos).c_str(), "%d%*[-//]%d", &vIndex, &vnIndex);
					line.erase(0, charPos + 1);
				}

				if (vIndex >= 1)
				{
					faceVertexIndicies.push_back(vIndex - 1);
				}
				if (uvIndex >= 1)
				{
					faceVertexTexCoordIndicies.push_back(uvIndex - 1);
				}
				if (vnIndex >= 1)
				{
					faceVertexNormalIndicies.push_back(vnIndex - 1);
				}
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
				log_warn(infoLog, fileName + " : " + "faceVertexIndices.size() : " + std::to_string(faceVertexIndicies.size()));
			}

			if (faceVertexTexCoordIndicies.size() == 3)
			{
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[0]]);
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[1]]);
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[2]]);
			}
			else if (faceVertexTexCoordIndicies.size() == 4)
			{
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[0]]);
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[1]]);
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[2]]);

				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[0]]);
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[2]]);
				vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[3]]);
			}
			else
			{
				GLchar infoLog[512] = { 0, };
				log_warn(infoLog, fileName + " : " + "vertexTexCoordIndices.size() : " + std::to_string(faceVertexTexCoordIndicies.size()));
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
				log_warn(infoLog, fileName + " : " + "faceVertexNormalIndices.size() : " + std::to_string(faceVertexNormalIndicies.size()));
			}
		}
	}

	ifs.close();

	return true;
}

Image *load_Image(std::string fileName, int *width, int *height, int *nrChannels)
{
	fileName = "../Images/" + fileName;
	stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load(fileName.c_str(), width, height, nrChannels, 0);
	if (data == NULL)
	{
		std::cout << "Failed to load texture : " + fileName << std::endl;
	}

	Image *img = new Image(*width, *height, *nrChannels, data);

	return img;
}

void free_image(Image *img)
{
	stbi_image_free(img->getData());
	free(img);
}