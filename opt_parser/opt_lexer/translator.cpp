#include "translator.h"

vector<int> translator::attributes = translator::SetAttributes();

void translator::GetError(string source, pair<int, int> position, string message) {
	cout << "[" << position.first << ", " << position.second << "] " << source << ": " << message << endl;
}

vector<int> translator::SetAttributes() {
	vector<int> local_list;
	local_list.assign(127, 6);
	string current_field;
	int line_iteration = 0, current_number = 0;
	fstream file("txt/attributes.txt", ios_base::in);
	if (file.is_open()) {
		while (file >> current_field) {
			if (current_field == ".")
				line_iteration++;
			else if (current_field == "-") {
				file >> current_field;
				for (int iteration = current_number + 1; iteration <= stoi(current_field); iteration++)
					local_list[iteration] = line_iteration;
			}
			else {
				current_number = stoi(current_field);
				local_list[current_number] = line_iteration;
			}
		}
	}
	return local_list;
}

translator::translator() {
	TableSetBasicInformation(keyword_table, 300);
	TableSetBasicInformation(identifier_table, 400);
	TableSetBasicInformation(constant_table, 500);
	TableSetBasicInformation(delimiter_table, 55);
	basic_attribures = identifier_table;
}

void translator::SetBasicInformation(const string name, vector<inform_table>* lst, int position) {
	vector<inform_table> inform_table_list;
	inform_table local_inform_table;
	fstream file("txt/" + name + ".txt", ios_base::in);
	if (file.is_open()) {
		while (getline(file, local_inform_table.word)) {
			local_inform_table.code = ++position;
			inform_table_list.push_back(local_inform_table);
		}
		*lst = inform_table_list;
	}
}

void translator::Lexer(string path) {
	fileReader file_object(path);
	char current_symbol;
	bool suppress_output = false;
	vector<int> acceptable_attributes;
	list<vector<inform_table>*> acceptable_tables;
	int current_value, current_attribute;
	if (file_object.FileIsSuitable())
		current_symbol = file_object.ReadSymbol();
	else {
		GetError("Lexer", file_object.GetCoordinates(), "invalid file name or content");
		return;
	}
	do {
		current_value = (int)current_symbol;
		current_attribute = attributes[current_value];
		switch (current_attribute) {
		case 0:
			current_symbol = ReadWithoutAdding(file_object.ReadSymbol(), &WhitespaceChecker, &file_object);
			break;
		case 1:
			current_symbol = ReadWithAdding(current_symbol, { 1 }, { &constant_table }, &file_object);
			break;
		case 2:
			current_symbol = ReadWithAdding(current_symbol, { 1, 2 }, { &keyword_table, &constant_table, &identifier_table }, &file_object);
			break;
		case 3:
			FindInTable(string(1, current_symbol), { &delimiter_table }, file_object.GetCoordinates());
			current_symbol = file_object.ReadSymbol();
			break;
		case 4:
			current_symbol = ReadComment(current_symbol, &file_object);
			break;
		default:
			GetError("Lexer", file_object.GetCoordinates(), "wrong symbol \"" + string(1, current_symbol) + "\"");
			current_symbol = file_object.ReadSymbol();
			break;
		}
	} while (!file_object.FileIsEof());
}

char translator::ReadComment(char start_symbol, fileReader* file_object) {
	char current_symbol;
	pair<int, int> start_coordinates = file_object->GetCoordinates();
	current_symbol = file_object->ReadSymbol();
	if (attributes[(int)current_symbol] == 5) {
		do {
			ReadWithoutAdding(current_symbol, &CommentChecker, file_object);
			current_symbol = file_object->ReadSymbol();
			if (file_object->FileIsEof()) {
				GetError("Lexer", file_object->GetCoordinates(), "end of comment is missing");
				return 0;
			}
		} while (current_symbol != ')');
		current_symbol = file_object->ReadSymbol();
	}
	else {
		FindInTable(string(1, start_symbol), { &delimiter_table }, start_coordinates);
	}
	return current_symbol;
}

char translator::ReadWithoutAdding(char current_symbol, bool (*Condition)(int), fileReader* file_object) {
	while (!file_object->FileIsEof() && Condition(attributes[(int)current_symbol])) {
		current_symbol = file_object->ReadSymbol();
	}
	return current_symbol;
}

void translator::FindInTable(string lexem_word, list<vector<inform_table>*> acceptable_tables, pair<int, int> coordinates) {
	for (vector<inform_table>* current_table_type : acceptable_tables) {
		for (inform_table current_inform_table : *current_table_type) {
			if (current_inform_table.word.compare(lexem_word) == 0) {
				lexem new_lexem = {
					coordinates,
					current_inform_table.code,
					lexem_word };
				lexem_table.push_back(new_lexem);
				return;
			}
		}
	}
	int new_code = acceptable_tables.back()->back().code + 1;
	acceptable_tables.back()->push_back(inform_table{ lexem_word, new_code });
	lexem_table.push_back(lexem{ coordinates, new_code, lexem_word });
}

char translator::ReadWithAdding(char symbol, vector<int> acceptable_attributes, list<vector<inform_table>*> acceptable_tables, fileReader* file_object) {
	string current_lexem;
	pair<int, int> start_coordinates = file_object->GetCoordinates();
	char current_symbol = symbol;
	do {
		current_lexem += current_symbol;
		current_symbol = file_object->ReadSymbol();
	} while (FindAttribute(attributes[(int)current_symbol], acceptable_attributes));
	FindInTable(current_lexem, acceptable_tables, start_coordinates);
	return current_symbol;
}

bool translator::WhitespaceChecker(int attribute) {
	return attribute == 0 ? true : false;
}

bool translator::CommentChecker(int attribute) {
	return attribute != 5 ? true : false;
}

bool translator::FindAttribute(int attribute, vector<int> attribute_list) {
	for (int current_attribute : attribute_list) {
		if (attribute == current_attribute) {
			return true;
		}
	}
	return false;
}

bool translator::TreeFindInTable(vector<tree_node>* node, vector<inform_table> table) {
	for (auto attribute : table)
		if (TreeAddIfSuitable(node, attribute.code)) {
			return true;
		}
	return false;
}

bool translator::TreeAddIfNotNull(vector<tree_node>* tree, tree_node current_node) {
	if (!current_node.rest.empty()) {
		tree->push_back(current_node);
		return true;
	}
	return false;
}

bool translator::TreeAddIfSuitable(vector<tree_node>* tree, int code_id) {
	if (lexem_table.size() > parser_position && lexem_table[parser_position].code == code_id) {
		tree->push_back({ lexem_table[parser_position].word });
		parser_position++;
		return true;
	}
	return false;
}

pair<int, int> translator::TreeGetPosition() {
	return lexem_table.size() > parser_position ? 
		lexem_table[parser_position].coordinates : 
		lexem_table[lexem_table.size() - 1].coordinates;
}

tree_node translator::Parser() {
	return SignalProgram();
}

tree_node translator::SignalProgram() {
	tree_node node;
	parser_position = 0;
	node.head = "<signal-program>";
	TreeAddIfNotNull(&node.rest, Program());
	return node;
}

tree_node translator::Program() {
	tree_node node;
	node.head = "<program>";
	if (!TreeAddIfSuitable(&node.rest, 301))
		return node;
	if (!TreeAddIfNotNull(&node.rest, ProcedureIdentifier())) {
		GetError("Parser", TreeGetPosition(), "identifier expected");
		return node;
	}
	TreeAddIfNotNull(&node.rest, ParametersList());
	if (!TreeAddIfSuitable(&node.rest, 56)) {
		GetError("Parser", TreeGetPosition(), "\";\" expected");
		return node;
	}
	if(!TreeAddIfNotNull(&node.rest, Block())) {
		GetError("Parser", TreeGetPosition(), "<block> expected");
		return node;
	}
	if (!TreeAddIfSuitable(&node.rest, 56)) {
		GetError("Parser", TreeGetPosition(), "\";\" expected");
		return node;
	}
	return node;
}

tree_node translator::Block() {
	tree_node node;
	node.head = "<block>";
	TreeAddIfNotNull(&node.rest, Declarations());
	if (!TreeAddIfSuitable(&node.rest, 302)) {
		GetError("Parser", TreeGetPosition(), "\"BEGIN\" expected");
		return node;
	}
	if (!TreeAddIfSuitable(&node.rest, 303)) {
		GetError("Parser", TreeGetPosition(), "\"END\" expected");
		return node;
	}
	return node;
}

tree_node translator::Declarations() {
	tree_node node;
	node.head = "<declarations>";
	TreeAddIfNotNull(&node.rest, VariableDeclarations());
	return node;
}

tree_node translator::VariableDeclarations() {
	tree_node node;
	node.head = "<variable-declarations>";
	if (!TreeAddIfSuitable(&node.rest, 304)) {
		return node;
	}
	if(!TreeAddIfNotNull(&node.rest, DeclarationsList())) {
		GetError("Parser", TreeGetPosition(), "identifier expected");
		return node;
	}
	return node;
}

tree_node translator::ProcedureIdentifier() {
	tree_node node;
	node.head = "<procedure-identifier>";
	TreeAddIfNotNull(&node.rest, Identifier());
	return node;
}

tree_node translator::Identifier() {
	tree_node node;
	node.head = "<identifier>";
	if (lexem_table[parser_position].code > 499) {
		GetError("Parser", TreeGetPosition(), "wrong identifier");
		return tree_node{};
	}
	if (TreeFindInTable(&node.rest, identifier_table) &&
		(lexem_table.size() > parser_position - 1)) {
		for (auto identifier : parser_identifiers) {
			if (identifier == lexem_table[parser_position - 1].code) {
				GetError("Parser", TreeGetPosition(), "identifier already declared");
				return node;
			}
		}
		parser_identifiers.push_back(lexem_table[parser_position - 1].code);
	}
	return node;
}

tree_node translator::ParametersList() {
	tree_node node;
	node.head = "<parameters-list>";
	if (!TreeAddIfSuitable(&node.rest, 57))
		return node;
	TreeAddIfNotNull(&node.rest, DeclarationsList());
	if (!TreeAddIfSuitable(&node.rest, 58)) {
		GetError("Parser", TreeGetPosition(), "\")\" expected");
		return node;
	}
	return node;
}

tree_node translator::DeclarationsList() {
	tree_node node;
	node.head = "<declarations-list>";
	if (TreeAddIfNotNull(&node.rest, Declaration()))
		TreeAddIfNotNull(&node.rest, DeclarationsList());
	return node;
}

tree_node translator::Declaration() {
	tree_node node;
	if (!TreeAddIfNotNull(&node.rest, VariableIdentifier()))
		return tree_node{};
	TreeAddIfNotNull(&node.rest, IdentifiersList());
	if (!TreeAddIfSuitable(&node.rest, 60)) {
		GetError("Parser", TreeGetPosition(), "\":\" expected");
		return tree_node{};
	}
	if(!TreeAddIfNotNull(&node.rest, Attribute())) {
		GetError("Parser", TreeGetPosition(), "attribute expected");
		return tree_node{};
	}
	TreeAddIfNotNull(&node.rest, AttributesList());
	if (!TreeAddIfSuitable(&node.rest, 56)) {
		GetError("Parser", TreeGetPosition(), "\";\" expected");
		return tree_node{};
	}
	node.head = "<declaration>";
	return node;
}

tree_node translator::VariableIdentifier() {
	tree_node node;
	node.head = "<variable-identifier>";
	TreeAddIfNotNull(&node.rest, Identifier());
	return node;
}

tree_node translator::IdentifiersList() {
	tree_node node;
	node.head = "<identifiers-list>";
	if (!TreeAddIfSuitable(&node.rest, 59))
		return node;
	if(TreeAddIfNotNull(&node.rest, VariableIdentifier()))
		TreeAddIfNotNull(&node.rest, IdentifiersList());
	return node;
}

tree_node translator::Attribute() {
	tree_node node;
	node.head = "<attribute>";
	TreeFindInTable(&node.rest, basic_attribures);
	return node;
}

tree_node translator::AttributesList() {
	tree_node node;
	node.head = "<attributes-list>";
	if (TreeAddIfNotNull(&node.rest, Attribute()))
		TreeAddIfNotNull(&node.rest, AttributesList());
	return node;
}

