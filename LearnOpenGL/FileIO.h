#ifndef FileIO_H

#define FileIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class FileIO
{
public:
	string read_file(const string path);
	void write_file(const string path, string contents);

private:
	ifstream inputStream;
	ofstream outputStream;
};

#endif