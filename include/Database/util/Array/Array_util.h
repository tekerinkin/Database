#pragma once
#include "Array.h"
#include <iostream>
#include <string>
#include <vector>

namespace util {

	template<typename T>
	size_t IndexOfMax(Array<T>& data) {
		size_t largest = 0;
		size_t size = data.Size();

		for(size_t i = 0; i < size; ++i) {
			if(data[i] > data[largest])
				largest = i;
		}

		return largest;
	}

	template<typename T>
	void InsertItem(Array<T>& data, size_t pos, const T& entry) {
		size_t size = data.Size();

		if(size + 1 > data.Capacity())
			data.Resize(2*data.Capacity());

		data.SetSize(size+1);

		for(size_t i = size; i > pos; --i) {
			data[i] = data[i-1];
		}

		data[pos] = entry;
	}

	template<typename T>
	size_t LowerBound(Array<T>& data, T entry) {
		size_t left = 0, right = data.Size();

		auto count = (right - left);

		while(count > 0) {
			auto mid = left + count/2;

			if(data[mid] < entry) {
				left = ++mid;
				count -= count/2 + 1;
			} else
				count /= 2;
		}

		return left;
	}

	template<typename T>
	size_t IndexOfFirstGreater(const Array<T>& data, const T& entry) {
		for(size_t i = 0; i < data.Size(); ++i) {
			if(entry < data[i])
				return i;
		}

		return data.Size();
	}

	template<typename T>
	void OrderedInsert(Array<T>& data, const T& entry) {
		size_t pos = IndexOfFirstGreater(data, entry);

		InsertItem(data, pos, entry);
	}

	template<typename T>
	void AttachArray(Array<T>& data1, Array<T>& data2) {
		auto size2 = data2.Size();

		for(int i = 0; i < size2; ++i)
			data1.PushBack(data2[i]);
	}

	template<typename T>
	void Split(Array<T>& data1, Array<T>& data2) {
		size_t size1 = data1.Size();
		size_t size2 = data2.Size();

		size_t size = size1;
		for(size_t i = ((size + 1)/2); i < size; ++i)
			data2[i - ((size+1)/2)] = data1[i];

		data1.SetSize((size+1)/2);
		data2.SetSize(size - data1.Size());
	}

	template<typename T>
	void PrintArray(Array<T>& data, int level = 0,
		std::ostream& out = std::cout, size_t spacing_size = 8) {

		out << std::string(level*spacing_size, ' ').c_str();
		out << '[' << ' ';
		for(size_t i = 0; i < data.Size(); ++i)
			out << data[i] << " ";
		out << ']';
	}

	template<typename T>
	bool IsGreater(const Array<T>& data, const T& item) {
		for(size_t i = 0; i < data.Size(); ++i) {
			if(item < data[i] || item == data[i])
				return false;
		}
		return true;
	}

	template<typename T>
	bool IsLess(const Array<T>& data, const T& item) {
		for(size_t i = 0;  i < data.Size(); ++i) {
			if(item > data[i] || item == data[i])
				return false;
		}

		return true;
	}

	template<typename T>
	void CopyArray(const Array<T>& src, Array<T>& dest) {
		auto size = src.Size();

		if(dest.Capacity() < size)
			dest.Resize(size*2);

		for(auto i = 0; i < size; ++i) {
			dest[i] = src[i];
		}

		dest.SetSize(size);
	}

	template<typename T>
	void AppendItem(Array<T>& arr, T entry) {
		arr.PushBack(entry);
	}

	template<typename T>
	T DeleteItem(Array<T>& arr, size_t index) {
		return arr.DeleteAtIndex(index);
	}
}