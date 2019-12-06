#pragma once

#include <string>

#include <glm/vec3.hpp>

class Image
{
private:
	int width;
	int height;
	int channel;

	unsigned char *data;

public:
	Image(int _width, int _height, int _cahnnel, unsigned char *_data);

	int getWidth();
	int getHeight();
	unsigned char *getData();
};

bool openObj(const std::string fileName, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &vertexNormals);
Image *load_Image(const std::string fileName, int *width, int *height, int *nrChannels);
