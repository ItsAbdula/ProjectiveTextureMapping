#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FileSystem
{
public:
	static std::string read_file(const std::string path);
	static void write_file(const std::string path, const std::string contents);

private:
	FileSystem() {}
};