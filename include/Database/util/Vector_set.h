#pragma once
#include "../Map/Map.h"
#include <vector>

namespace vset {

	template<typename T>
	std::vector<T> SetIntersect(const std::vector<T>& v1, const std::vector<T>& v2) {
		std::vector<T> result;
		Map::Map<T, int> map;

		for(T i : v1)
			map[i] = 0x2;
		for(T i : v2)
			map[i] |= 0x1;

		for(auto it = map.begin(); it != map.end(); ++it) {
			if(*it == 0x3)
				result.push_back(it.Key());
		}

		return result;
	}

	template<typename T>
	std::vector<T> SetUnion(const std::vector<T>& v1, const std::vector<T>& v2) {
		std::vector<T> result;
		db::BPTree<T> tree;
		for(T i : v1)
			tree.insert(i);
		for(T i : v2)
			tree.insert(i);

		for(auto it = tree.begin(); it != tree.end(); ++it)
			result.push_back(*it);

		return result;
	}
}