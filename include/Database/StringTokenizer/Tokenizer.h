#pragma once
#include "Token.h"
#include "StateTable.h"
#include <cstring>
#include <string>

class Tokenizer {
public:
	static const int kMaxBuffer = 255;

	Tokenizer();
	Tokenizer(const char* str);
	bool Done() {
		return pos_ >= strlen(buffer_);
	}

	bool Fail() {
		return state_ == -1;
	}

	friend Tokenizer& operator>>(Tokenizer& tokenizer, string_tokenizer::Token& token);

	void SetString(const char* str);


private:
	static const int kMaxCols = state_machine::kMaxCols;
	static const int kMaxRows = state_machine::kNumRows;

	int state_table_[kMaxRows][kMaxCols];
	int state_ = 0;
	char buffer_[kMaxBuffer] = "";
	size_t pos_ = 0;

	void MakeTable(int state_table[][kMaxCols]);

	bool GetToken(int start_state, std::string& token);
};