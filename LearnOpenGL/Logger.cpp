#include "Logger.h"

void log_error(char *buffer, const std::string contents)
{
	std::cout << "ERROR::" + contents + "\n" << buffer << std::endl;
}

void log_warn(char *buffer, const std::string contents)
{
	std::cout << "WARN::" + contents + "\n" << buffer << std::endl;
}