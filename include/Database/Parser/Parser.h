#pragma once
#include "../Map/Map.h"
#include "../MultiMap/Multimap.h"
#include "SQLStateMachine.h"
#include "Keywords.h"
#include "../Queue.h"
#include "../StringTokenizer/Tokenizer.h"
#include <cctype>
#include <exception>
#include <iostream>
#include <string>

class Parser {
public:
	Parser()
	{
		SetString("");
	}

	explicit Parser(const std::string& input)
	{
		SetString(input);
	}

	multi_map::MultiMap<std::string, std::string> Parse(const std::string& input);
private:
	queue::Queue<string_tokenizer::Token> tokens_;

	Tokenizer tokenizer_;

	sql_parser::SQLStateMachine sql_state_;

	void SetString(const std::string& input);

	const char* NO_CLOSING_QUOTE = "Syntax Error: no closing quote";
	const char* UNEXPECTED_TOKEN = "Syntax Error: unexpected token: ";
	const char* END_OF_INPUT = "Syntax Error: unexpected end of input";
};