#include "fileReader.h"

char fileReader::ReadSymbol() {
	char symbol;
	file.get(symbol);
	if ((int)symbol == 10) {
		coordinates.second++;
		coordinates.first = 0;
	}
	else
		coordinates.first++;
	return symbol;
}