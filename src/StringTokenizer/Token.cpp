#include <Database/StringTokenizer/Token.h>

using namespace string_tokenizer;

Token::Token() : token_(""), type_(0)
{}

Token::Token(const std::string& str, int type) : token_(str), type_(type)
{}

std::string Token::TypeString() const
{
	switch(type_) {
	case 1:
	case 7:
	case 11:
		return "ALPHA";
	case 2:
		return "SPACE";
	case 3:
	case 10:
		return "NUMBER";
	case 5:
	case 6:
		return "PUNCTUATION";
	case 9:
		return "ELLIPSIS";
	default:
		return "UNKNOWN";
	}
}