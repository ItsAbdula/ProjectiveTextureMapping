#include "FileIO.h"

string FileIO::read_file(const string path)
{
	inputStream.open(path);
	if (inputStream.is_open == false)
	{
		cerr << "Can't open : " << path << endl;
	}

	stringstream buffer;
	buffer << inputStream.rdbuf();
	auto contents(buffer.str());

	inputStream.close();

	return contents;
}

void FileIO::write_file(const string path, string contents)
{
	outputStream.open(path);
	if (outputStream.is_open == false)
	{
		cerr << "Can't open : " << path << endl;
	}

	outputStream << contents << endl;

	outputStream.close();
}