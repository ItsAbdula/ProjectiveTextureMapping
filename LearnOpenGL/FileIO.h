#ifndef FILEIO_H

#define FILEIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class FileIO
{
public:
	string read_file(string path);
	void write_file(string path, string contents);

private:
	ifstream inputStream;
	ofstream outputStream;
};

#endif