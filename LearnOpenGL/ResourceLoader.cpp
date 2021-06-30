#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ResourceLoader.h"

Image::Image(int _width, int _height, int _cahnnel, unsigned char* _data)
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

unsigned char* Image::getData()
{
    return data;
}

std::string get_extension(const std::string& filePath)
{
    return filePath.substr(filePath.find_last_of(".") + 1);
}

std::vector<int> tokenize_index(std::string& input, const char delim)
{
    std::vector<int> result;
    std::string token;
    std::stringstream ss(input);

    while (getline(ss, token, delim))
    {
        result.push_back(std::stoi(token));
    }

    return result;
}

glm::vec2 string_to_vec2(std::vector<std::string>& input)
{
    glm::vec2 result;

    result.x = stof(input.at(0));
    result.y = stof(input.at(1));

    return result;
}
glm::vec3 string_to_vec3(std::vector<std::string>& input)
{
    glm::vec3 result;

    result.x = stof(input.at(0));
    result.y = stof(input.at(1));
    result.z = stof(input.at(2));

    return result;
}

int parse_lines(std::ifstream& ifs, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& vertexTexCoord, std::vector<glm::vec3>& vertexNormals)
{
    std::string line;
    std::vector<std::string> tokens;
    std::string op;

    std::vector<glm::vec3> vertexIndices;
    std::vector<glm::vec2> vertexTexCoordIndices;
    std::vector<glm::vec3> vertexNormalIndices;

    while (std::getline(ifs, line))
    {
        if (line[0] == NULL || line[0] == '\n' || line[0] == '#' || line[0] == '!' || line[0] == '$' || line[0] == 'o' || line[0] == 'm' || line[0] == 'u') continue;

        tokens = parse_line(line);
        op = tokens.at(0);
        tokens.erase(tokens.begin() + 0);

        if (op.compare("v") == 0)
        {
            vertexIndices.push_back(string_to_vec3(tokens));
        }
        else if (op.compare("vt") == 0)
        {
            vertexTexCoordIndices.push_back(string_to_vec2(tokens));
        }
        else if (op.compare("vn") == 0)
        {
            vertexNormalIndices.push_back(string_to_vec3(tokens));
        }
        else if (op.compare("f") == 0)
        {
            std::vector<int> vertexInfo;

            std::vector<int> faceVertexIndicies;
            std::vector<int> faceVertexTexCoordIndicies;
            std::vector<int> faceVertexNormalIndicies;

            for (auto& elem : tokens)
            {
                vertexInfo = tokenize_index(elem, '/');

                if (vertexInfo.size() == 2) // maybe not included VertexTexCoord
                {
                    faceVertexIndicies.push_back(vertexInfo.at(0) - 1);
                    faceVertexNormalIndicies.push_back(vertexInfo.at(2) - 1);
                }
                else if (vertexInfo.size() == 3)
                {
                    faceVertexIndicies.push_back(vertexInfo.at(0) - 1);
                    faceVertexTexCoordIndicies.push_back(vertexInfo.at(1) - 1);
                    faceVertexNormalIndicies.push_back(vertexInfo.at(2) - 1);
                }
            }

            if (tokens.size() == 3)
            {
                vertices.push_back(vertexIndices[faceVertexIndicies[0]]);
                vertices.push_back(vertexIndices[faceVertexIndicies[1]]);
                vertices.push_back(vertexIndices[faceVertexIndicies[2]]);

                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[0]]);
                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[1]]);
                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[2]]);

                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[0]]);
                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[1]]);
                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[2]]);
            }
            else if (tokens.size() == 4) // rarely, face has 4 vertices
            {
                vertices.push_back(vertexIndices[faceVertexIndicies[0]]);
                vertices.push_back(vertexIndices[faceVertexIndicies[1]]);
                vertices.push_back(vertexIndices[faceVertexIndicies[2]]);
                vertices.push_back(vertexIndices[faceVertexIndicies[0]]);
                vertices.push_back(vertexIndices[faceVertexIndicies[2]]);
                vertices.push_back(vertexIndices[faceVertexIndicies[3]]);

                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[0]]);
                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[1]]);
                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[2]]);
                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[0]]);
                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[2]]);
                vertexTexCoord.push_back(vertexTexCoordIndices[faceVertexTexCoordIndicies[3]]);

                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[0]]);
                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[1]]);
                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[2]]);
                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[0]]);
                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[2]]);
                vertexNormals.push_back(vertexNormalIndices[faceVertexNormalIndicies[3]]);
            }
        }
    }

    return true;
}

std::vector<std::string> parse_line(std::string& line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;

    while (ss >> token)
    {
        result.push_back(token);
    }

    return result;
}

bool openObj(const std::string fileName, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& vertexTexCoord, std::vector<glm::vec3>& vertexNormals)
{
    vertices.clear();
    vertexTexCoord.clear();
    vertexNormals.clear();

    std::ifstream ifs;

    ifs.open("../Models/" + fileName);

    parse_lines(ifs, vertices, vertexTexCoord, vertexNormals);

    ifs.close();

    return true;
}

Image* make_Image(std::string fileName, int* width, int* height, int* nrChannels)
{
    fileName = "../Images/" + fileName;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(fileName.c_str(), width, height, nrChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture : " + fileName << std::endl;
    }

    Image* img = new Image(*width, *height, *nrChannels, data);

    return img;
}

void free_image(Image* img)
{
    stbi_image_free(img->getData());
    free(img);
}