#pragma once
#include <iostream>
#include <vector>

namespace util {
	template<typename T>
	std::string Print(const std::vector<T>& list) {
		std::string s = "";
		s += "[ ";
		for(size_t i = 0; i < list.size(); ++i) {
			s += list[i];
			if(i < list.size() - 1)
				s += ',';
			s += ' ';
		}
		s +=']';
		return s;
	}

	template<typename T>
	std::vector<T>& operator+=(std::vector<T>& list, const T& item) {
		list.push_back(item);
		return list;
	}
}