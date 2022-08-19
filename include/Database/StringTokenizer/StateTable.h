#pragma once
#include <cstddef>

namespace state_machine {
	const int kMaxCols = 255;
	const int kNumRows = 255;

	void InitTable(int table[][kMaxCols]);

	void MarkSuccess(int table[][kMaxCols], int state);

	void MarkFail(int table[][kMaxCols], int state);

	bool IsSuccess(int table[][kMaxCols], int state);

	void MarkCells(int row, int table[][kMaxCols], int form, int to, int state);

	void MarkCells(int row, int table[][kMaxCols], const char columns[], int state);

	void MarkCell(int row, int table[][kMaxCols], int column, int state);

	void MarkAll(int row, int table[][kMaxCols], int state);

	void UnmarkCell(int row, int table[][kMaxCols], int column);

	void PrintTable(int table[][kMaxCols]);

	void ShowString(const char* s, int pos);

	bool GetToken(const char* str, size_t& pos, int table[][kMaxCols], int& state);
}