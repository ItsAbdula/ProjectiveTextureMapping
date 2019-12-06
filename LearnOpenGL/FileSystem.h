#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FileSystem
{
public:
	std::string read_file(std::string path);
	void write_file(std::string path, std::string contents);

private:
	std::ifstream inputStream;
	std::ofstream outputStream;
};