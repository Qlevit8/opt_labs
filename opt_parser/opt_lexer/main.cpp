#include "view.h"

void LexerTrueTest() {
	translator translator_object = translator();

	view::PrintInformTable(translator_object.GetKeywords());
	view::PrintInformTable(translator_object.GetDelimiters());
	view::PrintAttributeTable(translator_object.GetAttributes());

	view::ShowFile("txt/lexer_true_test.txt");
	translator_object.Lexer("txt/lexer_true_test.txt");
	view::PrintLexemTable(translator_object.GetLexemTable());
	//view::PrintInformTable(translator_object.GetIdentifiers());
	//view::PrintInformTable(translator_object.GetConstants());
	//view::PrintInformTable(translator_object.GetDelimiters());
	//view::PrintInformTable(translator_object.GetKeywords());

	view::DrawTree(translator_object.Parser(), 0);
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
	LexerTrueTest();
	//LexerFalseTest();

	return 0;
}