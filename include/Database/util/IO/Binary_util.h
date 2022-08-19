#pragma once
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace bin_io {
	bool FileExists(const char* filename);
	void OpenFileRW(std::fstream& fs, const char* filename);
	void OpenFileW(std::fstream& fs, const char* filename);
}
