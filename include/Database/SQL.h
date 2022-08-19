#pragma once
#include "Parser/Parser.h"
#include "Table.h"
#include <iostream>
#include <string>

class SQL {
private:
	const char* tables_file_ = "tables.txt";

	Parser parser;
	Map::Map<std::string, Table> tables_;

	void GetTables();
	void PrintTableList();
	void PrintWelcome();

	const char* unknown_table_ =
		"Error: Unknown table see \"tables\" for available tables";

public:
	SQL();

	[[noreturn]] void Interactive();
	void ExecuteFile(const std::string& filename);
	bool ExecuteString(const std::string& str, bool verbose = false);
};