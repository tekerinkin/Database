#include <Database/Table.h>

const char* Table::temp = "temp";

Table::Table() : name_("") {}
Table::Table(const std::string& name) : name_(name) {
	// this shouldnt call on a nonexistent table
	if (!bin_io::FileExists(GetFilename().c_str()))
		throw std::runtime_error("Cannot find table");
	DBRead();
	Reindex();
}
Table::Table(const std::string& name, const std::vector<std::string>& columns) {
	// the technology isnt there for this yet :)
	if (columns.size() > constants::kMaxBlockRows) {
		throw std::runtime_error("Max field exceeded");
	}
	// check field lengths, because the technology isnt here for this yet
	for (size_t i = 0; i < columns.size(); ++i) {
		if (columns[i].size() >= constants::kMaxBlockCols)
			throw std::runtime_error("Max cols field exceeded" + columns[i]);
	}
	this->name_ = name;
	this->columns_ = columns;
	InitFile();
	DBRead();
}

bool Table::DBRead() {
	std::fstream file_stream;
	bin_io::OpenFileRW(file_stream, GetFilename().c_str());
	// if file is not empty
	if (file_stream.peek() != std::ifstream::traits_type::eof()) {
		Record index_block;
		index_block.Read(file_stream, 0);
		columns_ = index_block.ToVector();
	}
	// map the columns to make selecting them easier later on
	for (size_t i = 0; i < columns_.size(); ++i) {
		column_map_[columns_[i]] = i;
	}
	file_stream.close();
	return !file_stream.fail();
}

void Table::Append(const std::vector<std::string>& fields) {
	// check field lengths, because the technology isnt here for this yet
	for (size_t i = 0; i < fields.size(); ++i) {
		if (fields[i].size() >= constants::kMaxBlockCols)
			throw std::runtime_error("Max field size exceeded" + fields[i]);
	}
	if(!bin_io::FileExists(GetFilename().c_str()))
		throw std::runtime_error("Cannot find table");
	std::fstream f;
	bin_io::OpenFileW(f, GetFilename().c_str());
	if (fields.size() != columns_.size()) {
		throw std::runtime_error("Mismatched column number");
	}
	Record r;
	r.CreateFromVector(fields);
	int64_t record_number = r.AppendToFile(f);
	// update the index
	for (size_t i = 0; i < columns_.size(); ++i) {
		index_[columns_[i]][r.buffer_[i]].push_back(record_number);
	}
	f.close();
}

Table Table::Select(const std::vector<std::string>& fields,
	const std::vector<std::string>& where) {
	if (!bin_io::FileExists(GetFilename().c_str()))
		throw std::runtime_error("Cannot find table");
	std::fstream file_stream;
	bin_io::OpenFileRW(file_stream, GetFilename().c_str());

	// handle where condition
	std::vector<int64_t> where_indices;
	queue::Queue<std::string> where_queue = InfixToPostfix(where);
	where_indices = EvaluateWhere(where_queue);


	// this might need to be replaced with something else later
	if (fields[0] == "*") {
		Table tmp(temp, columns_);

		Record r;
		if (!where_indices.empty() || !where.empty()) {
			for (size_t i = 0; i < where_indices.size(); ++i) {
				r.Read(file_stream, where_indices[i]);
				tmp.Append(r.ToVector());
			}
		}
		else {
			for (int i = 1; r.Read(file_stream, i) > 0; ++i) {
				r.Read(file_stream, i);
				tmp.Append(r.ToVector());
			}
		}
		file_stream.close();
		return tmp;
	}
	else {
		for (size_t i = 0; i < fields.size(); ++i) {
			if (!column_map_.Contains(fields[i])) {
				throw std::runtime_error("Invalid name" + fields[i]);
			}
		}
		Table tmp(temp, fields);

		Record r;
		if (!where_indices.empty() || !where.empty()) {
			for (size_t i = 0; i < where_indices.size(); ++i) {
				std::vector<std::string> temp_row;
				r.Read(file_stream, where_indices[i]);
				for (size_t j = 0; j < fields.size(); ++j) {
					temp_row.push_back(r.buffer_[column_map_[fields[j]]]);
				}
				tmp.Append(temp_row);
			}
		}
		else {
			for (int64_t i = 1; r.Read(file_stream, i) > 0; ++i) {
				std::vector<std::string> temp_row;
				r.Read(file_stream, i);
				for (size_t j = 0; j < fields.size(); ++j) {
					temp_row.push_back(r.buffer_[column_map_[fields[j]]]);
				}
				tmp.Append(temp_row);
			}
		}
		file_stream.close();
		return tmp;
	}
	file_stream.close();

	return Table();
}

std::ostream& operator<<(std::ostream& outs, const Table& table) {
	outs << &table;
	return outs;
}

void Table::InitFile() {
	remove(GetFilename().c_str());
	// write fields to a block
	std::fstream f;
	bin_io::OpenFileW(f, GetFilename().c_str());
	Record r;
	r.CreateFromVector(columns_);
	// write block to beginning of the file
	r.Write(f, 0);
	f.close();
}

std::string Table::GetFilename() const { return (name_ + ".db"); }

void Table::Reindex() {
	std::fstream file_stream;
	file_stream.open(GetFilename());
	Record r;
	for (int64_t i = 1; r.Read(file_stream, i) > 0; ++i) {
		r.Read(file_stream, i);
		auto row = r.ToVector();
		multi_map::MultiMap<std::string, int64_t> mmap;
		for (size_t j = 0; j < columns_.size(); ++j) {
			index_[columns_[j]].Insert(row[j], i);
		}
	}
	file_stream.close();
}

std::vector<int64_t> Table::EvaluateWhere(queue::Queue<std::string>& where) {
	Stack<std::vector<int64_t>> results;
	while (!where.Empty()) {
		if (string_util::UpperCase(where.Front()) != "AND" &&
			string_util::UpperCase(where.Front()) != "OR") {

			std::string arg1 = where.Pop();
			std::string arg2 = where.Pop();
			std::string op = where.Pop();
			if (op == "=")
				results.Push(GetEqual(arg1, arg2));
			else if (op == "<")
				results.Push(GetLess(arg1, arg2));
			else if (op == "<=")
				results.Push(GetLessEqual(arg1, arg2));
			else if (op == ">")
				results.Push(GetGreater(arg1, arg2));
			else if (op == ">=")
				results.Push(GetGreaterEqual(arg1, arg2));
		} else {
			std::string logic = where.Pop();
			if (string_util::UpperCase(logic) == "AND") {
				results.Push(
					vset::SetIntersect(results.Pop(), results.Pop())
				);
			}
			else if (string_util::UpperCase(logic) == "OR") {
				results.Push(
					vset::SetUnion(results.Pop(), results.Pop())
				);
			}
		}
	}

	assert(results.Size() <= 1);
	if (!results.Empty())
		return results.Pop();
	else
		return std::vector<int64_t>();
}

std::vector<int64_t> Table::GetGreater (const std::string& arg1,const std::string& arg2) {
	if (!index_.Contains(arg1))
		throw std::runtime_error("Invalid name " + arg1);
	std::vector<int64_t> results;
	for (auto it = index_[arg1].UpperBound(arg2); it != index_[arg1].end(); ++it) {
		for (size_t i = 0; i < it->size(); ++i) {
			results.push_back(it->at(i));
		}
	}
	return results;
}
std::vector<int64_t> Table::GetGreaterEqual(const std::string& arg1,
	const std::string& arg2) {
	if (!index_.Contains(arg1))
		throw std::runtime_error("Invalid name " + arg1);
	std::vector<int64_t> results;
	for (auto it = index_[arg1].LowerBound(arg2); it != index_[arg1].end(); ++it) {
		for (size_t i = 0; i < it->size(); ++i) {
			results.push_back(it->at(i));
		}
	}
	return results;
}
std::vector<int64_t> Table::GetLess(const std::string& arg1,
	const std::string& arg2) {
	if (!index_.Contains(arg1))
		throw std::runtime_error("Invalid name " + arg1);
	std::vector<int64_t> results;
	multi_map::MultiMap<std::string, int64_t> _index = index_[arg1];
	for (auto it = _index.begin(); it != _index.LowerBound(arg2); ++it) {
		for (size_t i = 0; i < it->size(); ++i) {
			results.push_back(it->at(i));
		}
	}
	return results;
}
std::vector<int64_t> Table::GetLessEqual(const std::string& arg1,
	const std::string& arg2) {
	if (!index_.Contains(arg1))
		throw std::runtime_error("Invalid name " + arg1);
	std::vector<int64_t> results;
	multi_map::MultiMap<std::string, int64_t> _index = index_[arg1];
	for (auto it = _index.begin(); it != _index.UpperBound(arg2); ++it) {
		for (size_t i = 0; i < it->size(); ++i) {
			results.push_back(it->at(i));
		}
	}
	return results;
}
std::vector<int64_t> Table::GetEqual(const std::string& arg1,
	const std::string& arg2) {
	if (!index_.Contains(arg1))
		throw std::runtime_error("Invalid name " + arg1);
	std::vector<int64_t> results;
	if (index_[arg1].Contains(arg2)) {
		for (size_t i = 0; i < index_[arg1][arg2].size(); ++i) {
			results.push_back(index_[arg1][arg2][i]);
		}
	}
	return results;
}

std::ostream& Table::PrintTable(std::ostream& out) {
	const int num_width = 8;
	std::fstream file_stream;
	bin_io::OpenFileRW(file_stream, GetFilename().c_str());
	out << std::setw(num_width) << std::left << "Num";
	for (auto e : columns_) {
		out << std::setw(constants::kMaxBlockCols) << std::left;
		out << e;
	}
	out << '\n';
	out << std::string(columns_.size() * constants::kMaxBlockCols, '-');
	out << '\n';

	Record r;
	// sort by first column
	multi_map::MultiMap<std::string, int64_t> sorted = index_[columns_[0]];
	int num = 1;
	for (auto it = sorted.begin(); it != sorted.end(); ++it) {
		for (size_t i = 0; i < it->size(); ++i, ++num) {
			out << std::setw(num_width) << std::left << num;
			r.Read(file_stream, it->at(i));
			auto row = r.ToVector();
			for (size_t j = 0; j < row.size(); ++j) {
				out << std::setw(constants::kMaxBlockCols) << std::left;
				out << row[j];
			}
			out << '\n';
		}
	}
	file_stream.close();
	return out;
}