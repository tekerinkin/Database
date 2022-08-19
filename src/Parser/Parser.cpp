#include <Database/Parser/Parser.h>
#include <Database/util/Vector_util.h>

void Parser::SetString(const std::string& input) {
	tokens_.Clear();
	tokenizer_.SetString(input.c_str());
	string_tokenizer::Token t;
	while(!tokenizer_.Done()) {
		tokenizer_ >> t;
		tokens_.Push(t);
	}

	#ifdef DEBUG
		std::cout << tokens << std::endl;
	#endif
}

multi_map::MultiMap<std::string, std::string> Parser::Parse(const std::string& input)
{
	multi_map::MultiMap<std::string, std::string> parse_tree;

	SetString(input);
	sql_state_.ResetState();
	int last_state = sql_state_.GetState();

	while(!tokens_.Empty()) {
		string_tokenizer::Token t = tokens_.Pop();

		while(!tokens_.Empty() && (isspace(t.TokenStr()[0]) != 0))
			t = tokens_.Pop();

		if(isspace(t.TokenStr()[0]) != 0)
			break;

		std::string token_string = t.TokenStr();
		last_state = sql_state_.UpdateState(token_string);

		if(sql_state_.IsQuoteState(last_state)) {
			token_string = "";
			while(!tokens_.Empty() && sql_state_.IsQuoteState(last_state)) {
				std::string _token_string = tokens_.Pop().TokenStr();
				last_state = sql_state_.UpdateState(_token_string);
				if(_token_string != "\"")
					token_string += _token_string;
			}

			if(sql_state_.IsQuoteState(last_state))
				throw std::runtime_error(NO_CLOSING_QUOTE);
		}

		if(sql_state_.IsInvalid())
			throw std::runtime_error(UNEXPECTED_TOKEN + token_string);
		else {
			std::string key = sql_state_.GetParseKey(last_state);
			if(!key.empty()) {
				if(key == "command")
					parse_tree[key].push_back(string_util::UpperCase(token_string));
				else
					parse_tree[key].push_back(token_string);
			}
		}
	}

	if(sql_state_.IsInvalid() || !sql_state_.IsSuccess())
		throw std::runtime_error(END_OF_INPUT);

	#ifdef DEBUG
		std::cout << parse_tree << std::endl;
	#endif
	return parse_tree;
}