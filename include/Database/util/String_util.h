#pragma once
#include <string>
#include <vector>

namespace string_util {

	std::vector<std::string> Split(const std::string& str, const char& delim);

	std::string UpperCase(const std::string& str);

	bool IsWhitespace(const std::string& str);
}