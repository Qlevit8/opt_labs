#pragma once

#include <string>
#include <vector>
#include <list>
#include "fileReader.h"

#define TableSetBasicInformation(name, position) SetBasicInformation(#name, &name, position)

struct lexem {
	pair<int, int> coordinates;
	int code;
	string word;
};

struct inform_table {
	string word;
	int code;
};

class translator
{
private:
	vector<inform_table> keyword_table;
	vector<inform_table> identifier_table;
	vector<inform_table> constant_table;
	vector<inform_table> delimiter_table;
	vector<lexem> lexem_table;

	static vector<int> attributes;
	static vector<int> SetAttributes();

	bool FindAttribute(int, vector<int>);
	void SetBasicInformation(const string, vector<inform_table>*, int);
	char ReadWithAdding(char, vector<int>, list<vector<inform_table>*>, fileReader*);
	char ReadWithoutAdding(char, bool (*)(int), fileReader*);
	char ReadComment(char, fileReader*);
	static bool WhitespaceChecker(int);
	static bool CommentChecker(int);
	void FindInTable(string, list<vector<inform_table>*>, pair<int, int>);

public:
	translator();

	void Lexer(string path);
	void Parser();

	vector<inform_table> GetKeywords() { return keyword_table; };
	vector<inform_table> GetIdentifiers() { return identifier_table; };
	vector<inform_table> GetConstants() { return constant_table; };
	vector<inform_table> GetDelimiters() { return delimiter_table; };
	vector<lexem> GetLexemTable() { return lexem_table; };
	vector<int> GetAttributes() { return attributes; };
};