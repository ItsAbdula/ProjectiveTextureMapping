#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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
std::vector<int> tokenize_index(std::string &input, const char delim);
glm::vec2 string_to_vec2(std::vector<std::string> &input);
glm::vec3 string_to_vec3(std::vector<std::string> &input);
int parse_lines(std::ifstream &ifs, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &vertexTexCoord, std::vector<glm::vec3> &vertexNormals);
std::vector<std::string> parse_line(std::string &line);
bool openObj(const std::string fileName, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &vertexTexCoord, std::vector<glm::vec3> &vertexNormals);
Image *make_Image(std::string fileName, int *width, int *height, int *nrChannels);
void free_image(Image *img);