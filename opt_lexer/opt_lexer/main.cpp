#include "view.h"

void LexerTrueTest() {
	translator translator_object = translator();
	view::ShowFile("txt/lexer_true_test.txt");
	translator_object.Lexer("txt/lexer_true_test.txt");
	view::PrintLexemTable(translator_object.GetLexemTable());
	view::PrintInformTable(translator_object.GetIdentifiers());
	view::PrintInformTable(translator_object.GetConstants());
	view::PrintInformTable(translator_object.GetDelimiters());
	view::PrintInformTable(translator_object.GetKeywords());
}

void LexerFalseTest() {
	translator translator_object = translator();
	view::ShowFile("txt/lexer_false_test.txt");
	translator_object.Lexer("txt/lexer_false_test.txt");
	view::PrintLexemTable(translator_object.GetLexemTable());
	view::PrintInformTable(translator_object.GetIdentifiers());
	view::PrintInformTable(translator_object.GetConstants());
	view::PrintInformTable(translator_object.GetDelimiters());
	view::PrintInformTable(translator_object.GetKeywords());
}

int main() {

	//PrintInformTable(translator_object.GetKeywords());
	//PrintInformTable(translator_object.GetDelimiters());
	//PrintAttributeTable(translator_object.GetAttributes());

	//LexerTrueTest();
	LexerFalseTest();

	return 0;
}