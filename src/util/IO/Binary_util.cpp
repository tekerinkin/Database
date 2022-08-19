#include <Database/util/IO/Binary_util.h>

namespace bin_io {
	bool FileExists(const char* filename) {
		std::fstream fs;
		fs.open(filename, std::fstream::in | std::fstream::binary);
		if(fs.fail()) {
			#ifdef DEBUG
				std::cout << "file_exists(): File does NOT exist: " << filename << std::endl;
			#endif
			return false;
		}

		#ifdef DEBUG
			std::cout << "file_exists(): File DOES exist: " << filename << std::endl;
		#endif
		fs.close();
		return true;
	}

	void OpenFileRW(std::fstream& fs,const char* filename) {
		if(!FileExists(filename)) {
			fs.open(filename, std::fstream::out | std::fstream::binary);
			if(fs.fail()) {
				std::cout << "file open (RW) failed: with out|" << filename << "]"
						  << std::endl;
				throw std::runtime_error("file RW failed  ");
			} else {
				#ifdef DEBUG
					std::cout << "open_fileRW: file created successfully: " << filename
						  << std::endl;
				#endif
			}
		} else {
			fs.open(filename,
				std::fstream::in | std::fstream::out | std::fstream::binary);
			if (fs.fail()) {
				std::cout << "file open (RW) failed. [" << filename << "]" << std::endl;
				throw std::runtime_error("file failed to open.");
			}
		}
	}

	void OpenFileW(std::fstream& fs, const char* filename) {
		fs.open(filename, std::fstream::out | std::fstream::binary | std::ios::app);
		if (fs.fail()) {
			std::cout << "file open failed: " << filename << std::endl;
			throw std::runtime_error("file failed to open.");
		}
	}
}
