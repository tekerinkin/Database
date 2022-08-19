#pragma once
#include <Database/Constants.h>
#include <Database/util/IO/Binary_util.h>
#include <cstring>
#include <vector>

struct Record {
	static const int kRows = constants::kMaxBlockRows;
	static const int kCols = constants::kMaxBlockCols;

	int record_number_;
	char buffer_[kRows][kCols];

	Record();

	int64_t AppendToFile(std::fstream& fout);
	void Write(std::fstream& fout, int64_t index);
	int64_t Read(std::fstream& fout, int64_t record_number);

	bool WriteRow(const char* str);
	void ClearBuffer();

	std::vector<std::string> ToVector();

	void CreateFromVector(const std::vector<std::string>& items);

	friend std::ostream& operator<<(std::ostream& out, const Record& r);
};