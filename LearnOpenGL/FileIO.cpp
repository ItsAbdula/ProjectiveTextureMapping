#include "FileIO.h"

std::string FileIO::read_file(std::string path)
{
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

void FileIO::write_file(std::string path, std::string contents)
{
	outputStream.open(path);
	if (outputStream.is_open() == false)
	{
		std::cerr << "Can't open : " << path << std::endl;
	}

	outputStream << contents << std::endl;

	outputStream.close();
}