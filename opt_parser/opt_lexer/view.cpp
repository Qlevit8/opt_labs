#include "view.h"
#include <time.h>

void view::DrawTree(tree_node node, int depth) {
	for (int iter = 0; iter < depth; iter++)
		cout << " ";
	cout << node.head << endl;
	for (auto current_node : node.rest)
		DrawTree(current_node, depth + 1);
}

void view::ShowFile(string path) {
	fileReader file_object(path);
	char symbol = 10;
	cout << "Input file:\n";
	cout << "___________";
	while (!file_object.FileIsEof()) {
		cout << symbol;
		symbol = file_object.ReadSymbol();
	}
	cout << "\n___________\n";
}

void view::PrintLexemTable(vector<lexem> lexem_table) {
	cout << "+=========================================+\n";
	cout << "| Column|    Row|   Code|            Lexem|\n";
	cout << "|=========================================|\n";
	for (lexem current_lexem : lexem_table) {
		cout
			<< "|"
			<< setw(7) << current_lexem.coordinates.first << "|"
			<< setw(7) << current_lexem.coordinates.second << "|"
			<< setw(7) << current_lexem.code << "|"
			<< setw(17) << current_lexem.word << "|\n";
	}
	cout << "+=========================================+\n\n";
}

void view::PrintInformTable(vector<inform_table> lst) {
	cout << "+==============================+\n";
	cout << "|" << setw(20) << "Name|" << setw(10) << "Code" << "|\n";
	cout << "|==============================|\n";
	for (inform_table local_inform_table : lst) {
		cout << "|" << setw(20) << local_inform_table.word + "|" << setw(10) << local_inform_table.code << "|\n";
	}
	cout << "|==============================|\n\n";
}

void view::PrintAttributeTable(vector<int> attributes) {
	cout << "+=======================================================================+\n";
	for (int iteration = 32; iteration < attributes.size(); iteration++) {
		cout << "| " << attributes[iteration] << " " << setw(5) << (char)iteration;
		if (iteration % 8 == 7) {
			cout << "|" << endl;
		}
	}
	cout << "|\t|\n";
	cout << "+=======================================================================+\n\n";
}