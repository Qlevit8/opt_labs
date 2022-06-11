#pragma once

#include "translator.h"
#include <iomanip>

static class view
{
public:
	static void PrintInformTable(vector<inform_table>);
	static void PrintAttributeTable(vector<int>);
	static void PrintLexemTable(vector<lexem>);
	static void ShowFile(string);
};

