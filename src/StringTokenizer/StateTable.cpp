#include <Database/StringTokenizer/StateTable.h>
#include <iostream>

namespace state_machine {

	void InitTable(int table[][kMaxCols]) {
		for(int i = 0; i < kNumRows; ++i) {
			for(int j = 0; j < kMaxCols; ++j)
				table[i][j] = -1;
		}
	}

	void MarkSuccess(int table[][kMaxCols], int state) {
		table[state][0] = 1;
	}

	void MarkFail(int table[][kMaxCols], int state) {
		table[state][0] = 0;
	}

	bool IsSuccess(int table[][kMaxCols], int state) {
		return table[state][0] == 1;
	}

	void MarkCells(int row, int table[][kMaxCols], int from, int to, int state) {
		for(int i = from; i <= to; i++)
			table[row][i] = state;
	}

	void MarkCells(int row, int table[][kMaxCols], const char columns[], int state) {
		for(int i = 0; columns[i] != '\0'; i++)
			table[row][static_cast<int>(columns[i])] = state;
	}

	void MarkCell(int row, int table[][kMaxCols], int column) {
		table[row][column] = -1;
	}

	void MarkAll(int row, int table[][kMaxCols], int state) {
		for(int i = 1; i < kMaxCols; ++i)
			table[row][i] = state;
	}

	void UnmarkCell(int row, int table[][kMaxCols], int column) {
		table[row][column] = -1;
	}

	void PrintTable(int table[][kMaxCols]) {
		for(int i = 0; i < kNumRows; ++i) {
			for(int j = 0; j < kMaxCols; ++j)
				std::cout << table[i][j] << " ";
			std::cout << std::endl;
		}
	}

	void ShowString(const char* s, int pos) {
		std::cout << s << std::endl;
		for(int i = 0; i < pos; ++i)
			std::cout << " ";
		std::cout << '^' << std::endl;
		std::cout << "pos: " << pos;
	}

	bool GetToken(const char* str, size_t& pos, int table[][kMaxCols], int& state) {
		int pos_ = pos;
		bool token_found = false;
		while(str[pos_] != '\0' && state != -1) {
			state = table[state][static_cast<int>(str[pos_])];
			if(IsSuccess(table, state)) {
				pos = pos_;
				token_found = true;
			}
			pos_++;
		}
		return token_found;
	}
}