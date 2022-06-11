#pragma once

#include <fstream>
#include <iostream>

using namespace std;

class fileReader
{
private:
	pair<int, int> coordinates;
	fstream file;
public:

	fileReader() : fileReader("txt/file_1.txt") {};
	fileReader(string path) {
		coordinates.first = 0;
		coordinates.second = 1;
		file = fstream(path, ios_base::in);
	}

	~fileReader() {
		file.close();
	}

	bool FileIsEof() {
		return file.eof();
	}

	bool FileIsSuitable() {
		return file.is_open() && !FileIsEof() ? true : false;
	}

	char ReadSymbol();
	pair<int, int> GetCoordinates() { return coordinates; }
};

