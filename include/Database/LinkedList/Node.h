#pragma once
#include <iostream>

namespace l_list {

	template<typename T>
	struct Node {
		T item_;
		Node<T>* next_;
		explicit Node(const T& item = T(), Node<T>* next = nullptr)
			: item_(item), next_(next)
		{}

		friend std::ostream& operator<<(std::ostream& out, const Node<T>& print) {
			out << "[" << print.item_ << "]->";
			return out;
		}
	};
}