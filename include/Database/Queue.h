#pragma once
#include "LinkedList/l_list_util.h"
#include <iostream>

namespace queue {
	template<typename T>
	class Queue;

	template<typename T>
	class Iterator {
	public:
		friend class Queue<T>;
		explicit Iterator(l_list::Node<T>* node = nullptr) {
			this->node_ = node;
		}

		T& operator*() { return node_->item_;}
		T* operator->() {return &node_->item_;}

		friend Iterator operator++(Iterator& it, int) {
			Iterator temp;
			temp = it;
			it.node_ = it.node_->next_;
			return temp;
		}

		Iterator& operator++() {
			this->node_ = this->node_->next_;
			return *this;
		}

		friend bool operator !=(const Iterator& left, const Iterator& right) {
			return left.node_ != right.node_;
		}

		friend bool operator ==(const Iterator& left, const Iterator& right) {
			return left.node_ == right.node_;
		}

		friend bool operator !=(const Iterator& left, std::nullptr_t) {
			return left.node_ != nullptr;
		}

		friend bool operator ==(const Iterator& left, std::nullptr_t) {
			return left.node_ == nullptr;
		}

	private:
		l_list::Node<T>* node_;
	};

	template<typename T>
	class Queue {
	public:
		Queue() : first_(nullptr), last_(nullptr), size_(0)
		{}

		~Queue();

		Queue(const Queue<T>& other);
		Queue<T>& operator=(const Queue<T>& other);

		void Push(const T& item);
		T Pop();

		bool Empty() const {
			return size_ == 0;
		}

		size_t Size() const {
			return size_;
		}

		void Clear();

		T Front() { return this->first_->item_; }
		T Back() { return this->last_->item_; }

		Iterator<T> begin() const;
		Iterator<T> end() const;

		friend std::ostream& operator<<(std::ostream& out, const Queue& q) {
			return l_list::PrintList(q.first_, out);
		}

	private:
		l_list::Node<T>* first_;
		l_list::Node<T>* last_;
		size_t size_;
	};

	template<typename T>
	Queue<T>::Queue(const Queue<T>& other)
	{
		this->first_ = nullptr;
		this->last_ = l_list::CopyList(other.first_, this->first_);
		this->size_ = other.size_;
	}

	template<typename T>
	Queue<T>::~Queue()
	{
		l_list::DeleteAll(this->first_);
	}

	template<typename T>
	Queue<T>& Queue<T>::operator=(const Queue<T>& other)
	{
		if(this == &other)
			return *this;

		l_list::DeleteAll(this->first_);
		l_list::InitHead(this->first_);

		this->last_ = l_list::CopyList(other.first_, this->first_);
		this->size_ = other.size_;

		return *this;
	}

	template<typename T>
	void Queue<T>::Push(const T& item)
	{
		this->last_ =
			l_list::InsertAfter(this->first_, this->last_, item);
		this->size_++;
	}

	template<typename T>
	T Queue<T>::Pop()
	{
		if(this->size_ > 0) {
			this->size_--;
			return l_list::DeleteHead(this->first_);
		}

		return T();
	}

	template<typename T>
	Iterator<T> Queue<T>::begin() const
	{
		return Iterator<T>(this->first_);
	}

	template<typename T>
	Iterator<T> Queue<T>::end() const
	{
		return Iterator<T>(nullptr);
	}

	template<typename T>
	void Queue<T>::Clear()
	{
		l_list::DeleteAll(this->first_);
		this->last_ = nullptr;
		this->size_ = 0;
	}
}