#include "FileIO.h"

class FileIO
{
public:
	string read_file(string path)
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

	void write_file(string path, string contents)
	{
		outputStream.open(path);
		if (outputStream.is_open == false)
		{
			cerr << "Can't open : " << path << endl;
		}

		outputStream << contents << endl;

		outputStream.close();
	}

private:
	ifstream inputStream;
	ofstream outputStream;
};