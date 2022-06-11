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

struct tree_node {
	string head;
	vector<tree_node> rest;
};

class translator
{
private:
	vector<inform_table> keyword_table;
	vector<inform_table> identifier_table;
	vector<inform_table> constant_table;
	vector<inform_table> delimiter_table;
	vector<lexem> lexem_table;

	vector<inform_table> basic_attribures;
	vector<int> parser_identifiers;

	static vector<int> attributes;
	static vector<int> SetAttributes();
	static bool WhitespaceChecker(int);
	static bool CommentChecker(int);
	static void GetError(string, pair<int, int>, string);
	int parser_position = 0;

	bool TreeAddIfNotNull(vector<tree_node>*, tree_node);
	bool TreeAddIfSuitable(vector<tree_node>*, int);
	pair<int, int> TreeGetPosition();
	bool TreeFindInTable(vector<tree_node>*, vector<inform_table>);
	bool FindAttribute(int, vector<int>);
	void SetBasicInformation(const string, vector<inform_table>*, int);
	char ReadWithAdding(char, vector<int>, list<vector<inform_table>*>, fileReader*);
	char ReadWithoutAdding(char, bool (*)(int), fileReader*);
	char ReadComment(char, fileReader*);
	void FindInTable(string, list<vector<inform_table>*>, pair<int, int>);

	tree_node SignalProgram();
	tree_node Program();
	tree_node Block();
	tree_node ParametersList();
	tree_node Declarations();
	tree_node VariableDeclarations();
	tree_node DeclarationsList();
	tree_node Declaration();
	tree_node IdentifiersList();
	tree_node AttributesList();
	tree_node Attribute();
	tree_node VariableIdentifier();
	tree_node ProcedureIdentifier();
	tree_node Identifier();

public:
	translator();

	void Lexer(string path);
	tree_node Parser();

	vector<inform_table> GetKeywords() { return keyword_table; };
	vector<inform_table> GetIdentifiers() { return identifier_table; };
	vector<inform_table> GetConstants() { return constant_table; };
	vector<inform_table> GetDelimiters() { return delimiter_table; };
	vector<lexem> GetLexemTable() { return lexem_table; };
	vector<int> GetAttributes() { return attributes; };
};