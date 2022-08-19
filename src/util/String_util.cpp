#include <Database/util/String_util.h>

namespace string_util {

	std::vector<std::string> Split(const std::string& str, const char& delim) {
		std::vector<std::string> result;
		std::string token = "";

		for(size_t i = 0; i < str.size(); ++i) {
			if(str[i] != delim)
				token += str[i];
			else {
				result.push_back(token);
				token = "";
			}
		}
		return result;
	}

	std::string UpperCase(const std::string& str) {
		std::string str_ = str;
		for(size_t i = 0; i < str.size(); ++i) {
			str_[i] = toupper(str[i]);
		}
		return str_;
	}

	bool IsWhitespace(const std::string& str) {
		for(char c : str) {
			if(isspace(c) == 0)
				return false;
		}
		return true;
	}
}