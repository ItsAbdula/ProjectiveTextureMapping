#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Logger.h"

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

std::string get_extension(const std::string &filePath);
bool openObj(const std::string fileName, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &vertexTexCoord, std::vector<glm::vec3> &vertexNormals);
Image *load_Image(std::string fileName, int *width, int *height, int *nrChannels);
void free_image(Image *img);