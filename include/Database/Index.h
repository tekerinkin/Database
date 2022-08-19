#pragma once
#include "Map/Map.h"
#include "MultiMap/Multimap.h"
#include <iostream>
#include <string>

class Index {
public:
	friend std::ostream& operator<<(std::ostream& out, const Index& index);
	multi_map::MultiMap<std::string, int64_t>& operator[](const std::string& key);
	bool Contains(const std::string& field);

private:
	Map::Map<std::string, multi_map::MultiMap<std::string, int64_t>> index_;
};