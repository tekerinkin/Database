#pragma once
#include <iostream>
#include <exception>

namespace util {
	template<typename T>
	struct Array {

	Array() : data_()
	{}

	explicit Array(const T& item);
	explicit Array(size_t size);
	Array(const Array<T>& other);

	~Array() {
		delete[] data_;
	}

	Array<T>& operator=(const Array<T>& other);

	T& operator[](size_t index) const;

	friend std::ostream& operator<<(std::ostream& out, const Array<T>& arr) {
		out << "[ ";
		for(size_t i = 0; i < arr.size_; ++i) {
			out << arr[i] << " ";
		}
		out << "]";
		return out;
	}

	size_t Size() const{
		return size_;
	}

	void Resize(size_t capacity);

	void PushBack(T entry);

	void SetSize(size_t new_size) {
		size_ = new_size;
	}

	size_t Capacity() {
		return capacity_;
	}

	T PopBack();

	T DeleteAtIndex(size_t index);

	private:
		T* data_ = nullptr;
		size_t size_;
		size_t capacity_;

		void CopyFromArray(const T* src, size_t src_size);
	};

	template<typename T>
	Array<T>::Array(const T& item) {
		size_ = 1;
		capacity_ = 10;
		data_ = new T[capacity_]();
		data_[0] = item;
	}

	template<typename T>
	Array<T>::Array(size_t size) {
		size_ = size;
		capacity_ = 2*size_;
		data_ = new T[capacity_]();
	}

	template<typename T>
	Array<T>& Array<T>::operator=(const Array<T>& other)
	{
		if(this == &other)
			return *this;
		delete[] data_;
		data_ = new T[other.capacity_];
		capacity_ = other.capacity_;
		CopyFromArray(other.data_, other.size_);
		return *this;
	}

	template<typename T>
	Array<T>::Array(const Array<T>& other) {
		data_ = new T[other.capacity_];
		capacity_ = other.capacity_;
		CopyFromArray(other.data_, other.size_);
	}

	template<typename T>
	T& Array<T>::operator[](size_t index) const {
		if(index >= size_ || index < 0)
			throw std::out_of_range("Array: Out of range");
		else
			return data_[index];
	}

	template<typename T>
	void Array<T>::CopyFromArray(const T* src, size_t src_size)
	{
		size_ = src_size;
		for(size_t i = 0; i < size_; ++i) {
			data_[i] = src[i];
		}
	}

	template<typename T>
	void Array<T>::Resize(size_t capacity)
	{
		auto temp_data = new T[capacity]();

		for(size_t i = 0; i < size_; ++i)
			temp_data[i] = data_[i];

		delete[] data_;
		data_ = temp_data;
		capacity_ = capacity;
	}

	template<typename T>
	void Array<T>::PushBack(T entry)
	{
		if(size_ + 1 > capacity_) {
			Resize(2*capacity_);
		}

		data_[++size_] = entry;
	}

	template<typename T>
	T Array<T>::PopBack()
	{
		if(size_ == 0)
			throw std::out_of_range("PopBack: Out of range");

		auto ret = data_[--size_];
		data_[size_+1] = 0;
		return ret;
	}

	template<typename T>
	T Array<T>::DeleteAtIndex(size_t index)
	{
		if(size_ == 0 || index >= size_ || index < 0)
			throw std::out_of_range("DeleteAtIndex: Out of range");
		auto ret = data_[index];

		for(size_t i = index; i < size_ - 1; ++i)
			data_[i] = data_[i+1];
		data_[size_-1] = 0;
		--size_;

		return ret;
	}
}