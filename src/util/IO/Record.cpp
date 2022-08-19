#include <Database/util/IO/Record.h>

Record::Record() {
	ClearBuffer();
	record_number_ = -1;
}

int64_t Record::AppendToFile(std::fstream& fout)
{
	fout.seekp(0, std::ios::end);
	int64_t pos = fout.tellp() / (kRows * kCols);

	for(int i = 0; i < kRows; ++i)
		fout.write(buffer_[i], kCols);

	return pos;
}

void Record::Write(std::fstream& fout, int64_t index) {
	fout.seekp(kRows * kCols * index, std::ios::beg);

	for(int i = 0; i < kRows; ++i)
		fout.write(buffer_[i], kCols);
}

int64_t Record::Read(std::fstream& fout, int64_t record_number)
{
	ClearBuffer();
	this->record_number_ = record_number;
	fout.seekg(kRows*kCols*record_number);

	for(int i = 0; i < kRows; ++i)
		fout.read(buffer_[i], kCols);

	return fout.gcount();
}

std::ostream& operator<<(std::ostream& out, const Record& r) {
	for(int i = 0; i < r.kRows; ++i) {
		for(int j = 0; j < r.kCols; ++j)
			out << r.buffer_[i][j];
		out << '\n';
	}
	return out;
}

bool Record::WriteRow(const char* str)
{
	for(int i = 0; i < kRows; ++i) {
		if(buffer_[i][0] == '\0') {
			strcpy(buffer_[i], str);
			return true;
		}
	}

	return false;
}

std::vector<std::string> Record::ToVector()
{
	std::vector<std::string> row;
	for(int i = 0;i < kRows && buffer_[i][0] != '\0'; ++i) {
		char field[kCols] = "";
		strcpy(field, buffer_[i]);
		row.push_back(field);
	}

	return row;
}

void Record::ClearBuffer()
{
	for(int i = 0; i < kRows; ++i) {
		for(int j = 0; j < kCols; ++j)
			buffer_[i][j] = '\0';
	}
}

void Record::CreateFromVector(const std::vector<std::string>& items)
{
	ClearBuffer();
	if(items.size() > kCols)
		throw std::runtime_error("Error: max number of items exceeded");

	for(size_t i = 0; i < items.size(); ++i)
		WriteRow(items.at(i).c_str());
}