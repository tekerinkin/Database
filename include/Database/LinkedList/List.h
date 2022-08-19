#pragma once
#include <Database/LinkedList/l_list_util.h>
#include <iostream>

namespace l_list {
	template<typename T>
	class List;


	template<typename T>
	class Iterator {
	public:
		friend class List<T>;

		explicit Iterator(Node<T>* node = nullptr) {
			this->node_ = node;
		}

		T& operator*() {
			return node_->item_;
		}

		friend Iterator operator++(Iterator& it, int) {
			Iterator temp;
			temp = it;
			it.node_= it.node_->next_;
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

	private:
		Node<T>* node_;
	};

	template<typename T>
	class List {
	public:
		List() : first_(nullptr), last_(nullptr), size_(0)
		{}
		~List();
		List(const List<T>& other);
		List<T>& operator=(const List<T>& other);

		Iterator<T> Find(const T& item) const;

		void PushFront(const T& item);
		void PushBack(const T& item);
		T PopFront();

		void Insert(size_t pos, const T& item);
		void InsertAfter(Iterator<T> pos, const T& item);
		void InsertAfter(Node<T>* pos, const T& item);

		bool Empty() const {
			return size_ == 0;
		}

		size_t Size() const {
			return size_;
		};

		void Clear();

		T Front() {
			return this->first_->item_;
		}

		T Back() {
			return this->last_->item_;
		}

		Iterator<T> begin() const;
		Iterator<T> end() const;

		friend std::ostream& operator<<(std::ostream& out, const List& list) {
			for(auto it = list.begin(); it != list.end(); it++)
				out << *it << " ";
			return out;
		}

	private:
		Node<T>* first_;
		Node<T>* last_;
		size_t size_;
	};

	template<typename T>
	List<T>::List(const List<T>& other) {
		this->data_ = nullptr;
		this->first_ = CopyList(other.first_, this->first_);
		this->size_ = other.size_;
	}

	template<typename T>
	List<T>::~List() {
		DeleteAll(this->first_);
	}

	template<typename T>
	List<T>& List<T>::operator=(const List<T>& other)
	{
		if(this == &other)
			return *this;
		DeleteAll(this->first_);
		InitHead(this->first_);

		this->last_ = CopyList(other.first_, this->first_);
		this->size_ = other.size_;

		return *this;
	}

	template<typename T>
	void List<T>::PushFront(const T& item)
	{
		InsertHead(this->first_, item);
		if(this->last_ == nullptr)
			this->last_ = this->first_;
		this->size_++;
	}

	template<typename T>
	void List<T>::PushBack(const T& item)
	{
		this->last_ = InsertAfter(this->first_, this->last_, item);
		this->size_++;
	}

	template<typename T>
	T List<T>::PopFront()
	{
		if(this->size_ > 0) {
			this->size_--;
			return DeleteHead(this->first_);
		}

		return T();
	}

	template<typename T>
	Iterator<T> List<T>::Find(const T& item) const
	{
		return Iterator<T>(l_list::Find(this->first_, item));
	}

	template<typename T>
	Iterator<T> List<T>::begin() const
	{
		return Iterator<T>(this->first_);
	}

	template<typename T>
	Iterator<T> List<T>::end() const {
		return Iterator<T>(nullptr);
	}

	template<typename T>
	void List<T>::Insert(size_t pos, const T& item){
		if(pos == 0) {
			this->PushFront(item);
			return;
		}

		Node<T>* walk = this->first_;
		for(size_t i = 0; i < pos; i++, walk = walk->next_) {
			if(walk == nullptr && i != pos - 1)
				throw std::out_of_range("List::Insert");

			if(walk == nullptr && i == pos - 1)
				this->PushBack(item);
			else {
				if(i == pos - 1)
					InsertAfter(this->first_, walk, item);
			}
		}
	}

	template<typename T>
	void List<T>::InsertAfter(Iterator<T> pos, const T& item)
	{
		this->InsertAfter(pos.node_, item);
	}

	template<typename T>
	void List<T>::InsertAfter(Node<T>* pos, const T& item)
	{
		InsertAfter(this->data_, pos, item);
	}

	template<typename T>
	void List<T>::Clear() {
		DeleteAll(this->first_);
		this->last_ = nullptr;
		this->size_ = 0;
	}
}