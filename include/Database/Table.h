#pragma once
#include "Index.h"
#include "Map/Map.h"
#include "MultiMap/Multimap.h"
#include "Constants.h"
#include "ShuntingYard.h"
#include "util/IO/Record.h"
#include "util/Vector_set.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


class Table {
public:
	Table();

	Table(const std::string& name);

	Table(const std::string& name, const std::vector<std::string>& columns);

	void Append(const std::vector<std::string>& fields);

	Table Select(const std::vector<std::string>& fields, const std::vector<std::string>& where);

	void InitFile();

	std::string GetFilename() const;

	friend std::ostream& operator<<(std::ostream& out, const Table& table);

	std::ostream& PrintTable(std::ostream& out = std::cout);

	static const char* temp;
private:
	std::string name_;

	std::vector<std::string> columns_;
	Map::Map<std::string, size_t> column_map_;
	Index index_;

	bool DBRead();

	void Reindex();

	std::vector<int64_t> EvaluateWhere(queue::Queue<std::string>& where);

	std::vector<int64_t> GetGreater(const std::string& arg1, const std::string& arg2);

	std::vector<int64_t> GetLess(const std::string& arg1, const std::string& arg2);

	std::vector<int64_t> GetGreaterEqual(const std::string& arg1, const std::string& arg2);

	std::vector<int64_t> GetLessEqual(const std::string& arg1, const std::string& arg2);

	std::vector<int64_t> GetEqual(const std::string& arg1, const std::string& arg2);

	const char* cannot_find_table_ = "Error: cannot find specified table";
	const char* max_field_exceeded_ = "Error: Max number of fields exceeded";
	const char* max_field_size_exceeded_ = "Error: Max field size exceeded at: ";
	const char* mismatched_col_num_ = "Error: mismatched column number";
	const char* invalid_name_ = "Error: Invalid name: ";
};