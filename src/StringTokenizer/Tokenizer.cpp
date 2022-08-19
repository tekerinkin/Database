#include <Database/StringTokenizer/Tokenizer.h>

Tokenizer::Tokenizer() {
	MakeTable(state_table_);
}

Tokenizer::Tokenizer(const char* str)
{
	MakeTable(state_table_);
	strcpy(buffer_, str);
}

void Tokenizer::MakeTable(int (*state_table)[255])
{
	const char* ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
	const char* NUMBERS = "1234567890";
	const char* PUNCTUATION = ",\\.?!;\"'()[]{}/";
	const char* COMPARISON = "<>";
	const char* WHITESPACE = " \t\n\r";
	
	state_machine::InitTable(state_table);

	state_machine::MarkFail(state_table,       0);
	state_machine::MarkSuccess(state_table,    1);
	state_machine::MarkSuccess(state_table,    2);
	state_machine::MarkSuccess(state_table,    3);
	state_machine::MarkFail(state_table,       4);
	state_machine::MarkSuccess(state_table,    5);
	state_machine::MarkFail(state_table,       6);
	state_machine::MarkSuccess(state_table,    7);
	state_machine::MarkFail(state_table,       8);
	state_machine::MarkSuccess(state_table,    9);
	state_machine::MarkSuccess(state_table,    10);
	state_machine::MarkSuccess(state_table,    11);
	state_machine::MarkSuccess(state_table,    12);
	state_machine::MarkCells(0, state_table, ALPHA, 1);
	state_machine::MarkCells(1, state_table, ALPHA, 1);
	state_machine::MarkCells(2, state_table, ALPHA, -1);
	state_machine::MarkCells(3, state_table, ALPHA, -1);
	state_machine::MarkCells(4, state_table, ALPHA, -1);
	state_machine::MarkCells(5, state_table, ALPHA, -1);
	state_machine::MarkCells(6, state_table, ALPHA, 1);
	state_machine::MarkCells(0, state_table, WHITESPACE, 2);
	state_machine::MarkCells(1, state_table, WHITESPACE, -1);
	state_machine::MarkCells(2, state_table, WHITESPACE, 2);
	state_machine::MarkCells(0, state_table, NUMBERS, 3);
	state_machine::MarkCells(1, state_table, NUMBERS, 1);
	state_machine::MarkCells(2, state_table, NUMBERS, -1);
	state_machine::MarkCells(3, state_table, NUMBERS, 3);
	state_machine::MarkCells(4, state_table, NUMBERS, 3);
	state_machine::MarkCells(10, state_table, NUMBERS, 10);

	state_machine::MarkCells(0, state_table, PUNCTUATION, 5);

	state_machine::MarkCell(0, state_table, '.', 5);
	state_machine::MarkCell(1, state_table, '.', -1);
	state_machine::MarkCell(2, state_table, '.', -1);
	state_machine::MarkCell(3, state_table, '.', 10);
	state_machine::MarkCell(4, state_table, '.', -1);
	state_machine::MarkCell(5, state_table, '.', 8);
	state_machine::MarkCell(6, state_table, '.', -1);
	state_machine::MarkCell(7, state_table, '.', -1);
	state_machine::MarkCell(8, state_table, '.', 9);

	state_machine::MarkCell(0, state_table, '\'', 6);
	state_machine::MarkCell(1, state_table, '\'', 6);

	state_machine::MarkCells(0, state_table, COMPARISON, 11);
	state_machine::MarkCell(0, state_table, '=', 12);
	state_machine::MarkCell(11, state_table, '=', 12);
}

bool Tokenizer::GetToken(int start_state, std::string& token)
{
	size_t last_pos = pos_;
	bool has_token = state_machine::GetToken(buffer_, pos_, state_table_, start_state);
	char token_[kMaxBuffer] = "";
	strncpy(token_, buffer_ + last_pos, pos_ + 1 - last_pos);
	token = token_;
	pos_ += 1;
	return has_token;
}

void Tokenizer::SetString(const char* str)
{
	pos_ = 0;
	state_ = 0;
	strcpy(buffer_, str);
}

Tokenizer& operator>>(Tokenizer& tokenizer, string_tokenizer::Token& t) {
	std::string str;
	tokenizer.GetToken(tokenizer.state_, str);
	int state = 0;
	size_t pos = 0;
	state_machine::GetToken(str.c_str(), pos, tokenizer.state_table_, state);
	t.SetType(state);
	t.SetString(str);
	return tokenizer;
}