#pragma once
#include <Database/LinkedList/Node.h>
#include <iostream>

namespace l_list {

	template<typename T>
	Node<T>* InitHead(Node<T>* head) {
		head = nullptr;
		return head;
	}

	template<typename T>
	void DeleteAll(Node<T>* head) {
		if(head == nullptr)
			return;
		if(head->next_ != nullptr)
			DeleteAll(head->next_);
		delete head;
		head = nullptr;
	}

	template<typename T>
	bool Empty(const Node<T>* head) {
		return head == nullptr;
	}

	template<typename T>
	Node<T>* InsertHead(Node<T>* head, const T& item) {
		Node<T>* node;

		if(head == nullptr) {
			node = new Node<T>(item);
			head = node;
		} else {
			node = new Node<T>(item, head);
			head = node;
		}

		return node;
	}

	template<typename T>
	Node<T>* CopyList(const Node<T>* src, Node<T>* dest) {

		if(src == nullptr)
			return nullptr;
		dest = InsertHead(dest, src->item_);
		if(src->next_ == nullptr)
			return dest;
		return CopyList(src->next_, dest->next_);
	}

	template<typename T>
	Node<T>* InsertAfter(Node<T>* head, Node<T>* after, const T& item) {
		if(head == nullptr)
			return InsertHead(head, item);
		return InsertHead(after->next_, item);
	}

	template<typename T>
	T DeleteHead(Node<T>* head) {
		if(head == nullptr)
			return T();

		Node<T>* temp = head;
		T temp_item = head->item_;
		head = head->next_;
		delete temp;
		return temp_item;
	}

	template<typename T>
	Node<T>* PreviousNode(Node<T>* head, Node<T>* node) {
		if(head == nullptr)
			throw std::runtime_error("LList::PreviousNode");
		if(head == node)
			return nullptr;
		if(head->next_ == node)
			return head;
		return PreviousNode(head->next_, node);
	}

	template<typename T>
	Node<T>* Find(Node<T>* head, const T& item) {
		if(head == nullptr)
			return nullptr;
		if(head->item_ == item)
			return head;
		return Find(head->next_, item);
	}

	template<typename T>
	std::ostream& PrintList(const Node<T>* head, std::ostream& out = std::cout) {
		if(head == nullptr)
			return out;
		else {
			out << head->item_ << " ";
			return PrintList(head->next_);
		}
	}
}