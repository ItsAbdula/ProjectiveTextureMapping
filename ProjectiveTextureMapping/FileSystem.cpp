#include "FileSystem.h"

std::string FileSystem::read_file(const std::string path)
{
    std::ifstream inputStream;

    inputStream.open(path);
    if (inputStream.is_open() == false)
    {
        std::cerr << "Can't open : " << path << std::endl;
    }

    std::stringstream buffer;
    buffer << inputStream.rdbuf();
    auto contents(buffer.str());

    inputStream.close();

    return contents;
}

void FileSystem::write_file(const std::string path, const std::string contents)
{
    std::ofstream outputStream;

    outputStream.open(path);
    if (outputStream.is_open() == false)
    {
        std::cerr << "Can't open : " << path << std::endl;
    }

    outputStream << contents << std::endl;

    outputStream.close();
}