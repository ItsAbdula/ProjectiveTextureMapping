#include <iostream>
#include <string>

#include "Logger.h"

void log_error(char *buffer, const std::string contents)
{
	std::cout << "ERROR::" + contents + "\n" << buffer << std::endl;
}