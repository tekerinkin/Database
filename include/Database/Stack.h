#pragma once
#include "LinkedList/List.h"
#include <iostream>

template<typename T>
class Stack {
public:
	void Push(const T& item) {
		this->list_.PushFront(item);
	}

	T Pop() {
		return this->list_.PopFront();
	}

	T Top() {
		return this->list_.Front();
	}

	bool Empty() const {
		return this->list_.Empty();
	}

	size_t Size() const {
		return this->list_.Size();
	}

	void Clear() {
		this->list_.Clear();
	}

	friend std::ostream& operator<<(std::ostream& out, const Stack& stack) {
		return out << stack.list_;
	}
private:
	l_list::List<T> list_;
};