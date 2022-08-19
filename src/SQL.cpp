#include <Database/SQL.h>

SQL::SQL() {
	PrintWelcome();
	GetTables();
}

[[noreturn]] void SQL::Interactive()
{
	std::string command;
	while(true) {
		std::cout << "sql> ";
		getline(std::cin, command);
		ExecuteString(command);
	}
}

bool SQL::ExecuteString(const std::string& str, bool verbose)
{
	try {
		auto parse_tree = parser.Parse(str);
#ifdef DEBUG
		std::cout << parse_tree << '\n';
#endif

		std::string command;
		std::string table_name;
		std::vector<std::string> fields;
		if (!parse_tree["command"].empty())
			command = parse_tree["command"][0];
		if (!parse_tree["table_name"].empty())
			table_name = parse_tree["table_name"][0];
		if (!parse_tree["fields"].empty())
			fields = parse_tree["fields"];

		if (table_name == Table::temp) {
			throw std::runtime_error("Illegal name");
		}
		// select
		if (command == "SELECT") {
			if (!tables_.Contains(table_name)) {
				throw std::runtime_error("Unknown table");
			} else {
				tables_[Table::temp] =
					tables_[table_name].Select(fields, parse_tree["where"]);
				tables_[Table::temp].PrintTable() << '\n';
			}
		}
		// create
		else if (command == "CREATE" || command == "MAKE") {
			if (!tables_.Contains(table_name)) {
				std::ofstream fout;
				fout.open(tables_file_, std::ios::app);
				fout << '\n' << table_name;
				fout.close();
			}
			tables_[table_name] = Table(table_name, fields);
			if (verbose) {
				tables_[table_name].PrintTable() << '\n';
			}
		}
		// insert
		else if (command == "INSERT") {
			if (!tables_.Contains(table_name)) {
				throw std::runtime_error("Unknown table");
			}
			else {
				tables_[table_name].Append(fields);
				if (verbose) {
					tables_[table_name].PrintTable() << '\n';
				}
			}
		}
		// drop
		else if (command == "DROP") {
			if (tables_.Contains(table_name)) {
				remove(tables_[table_name].GetFilename().c_str());
				tables_.Erase(table_name);
				std::ofstream fout;
				fout.open(tables_file_);
				for (auto it = tables_.begin(); it != tables_.end(); ++it) {
					fout << it.Key() << '\n';
				}
				fout.close();
			}
			else {
				throw std::runtime_error("Unknown table");
			}
		}
		// print tables command
		else if (command == "TABLES") {
			PrintTableList();
		}
		// exits the program
		else if (command == "EXIT") {
			if (tables_.Contains(Table::temp))
				ExecuteString("DROP TABLE " + std::string(Table::temp));
			exit(0);
		}
		// executes a given file
		else if (command == "EXEC") {
			std::string filename = "";
			for (size_t i = 0; i < fields.size(); ++i) {
				if (fields[i] != "\"")
					filename += fields[i];
			}
			ExecuteFile(filename);
		}
		return true;
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << '\n';
		return false;
	}
}