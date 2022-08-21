#pragma once
#include <cassert>
#include <exception>
#include <iostream>
#include <vector>

#include <Database/util/Array/Array.h>
#include <Database/util/Array/Array_util.h>

namespace db {

	template<typename T>
	class BPTree;

	template<typename T>
	struct Iterator {
		constexpr explicit Iterator(BPTree<T>* node = nullptr, size_t offset = 0)
			:node_(node), offset_(offset)
		{}

		T operator*() {
			if(offset_ > node_->Size())
				throw std::out_of_range("Iterator is out of range");
			if(node_ == nullptr)
				throw std::invalid_argument("Iterator is null");
			return node_->GetItem(offset_);
		}

		T* operator->() {
			if(offset_ > node_->Size())
				throw std::out_of_range("Iterator is out of range");
			return &node_->GetItem(offset_);
		}

		Iterator operator++(int) {
			Iterator temp = *this;
			if(offset_ < node_->Size() - 1)
				offset_++;
			else {
				node_ = node_->GetNext();
				offset_ = 0;
			}

			return temp;
		}

		Iterator operator++() {
			if(offset_ < node_->Size() - 1)
				offset_++;
			else {
				node_ = node_->GetNext();
				offset_ = 0;
			}
			return *this;
		}

		friend bool operator==(const Iterator& left, const Iterator& right) {
			return left.node_ == right.node_ && left.offset_ == right.offset_;
		}

		friend bool operator!=(const Iterator& left, const Iterator& right) {
			return left.node_ != right.node_ || left.offset_ != right.offset_;
		}

		friend bool operator==(const Iterator& left, std::nullptr_t) {
			return left.node_ == nullptr;
		}

		friend bool operator!=(const Iterator& left, std::nullptr_t) {
			return left.node_ != nullptr;
		}

		bool IsNull() {
			return node_ == nullptr;
		}

	private:
		BPTree<T>* node_;
		size_t offset_;
	};


	template<typename T>
	class BPTree {
	public:

		explicit BPTree(bool duplicate_allowed = false);
		BPTree(const BPTree<T>& other);
		~BPTree();
		BPTree<T>& operator=(const BPTree<T>& other);

		void Insert(const T& entry);
		void Remove(const T& entry);

		void ClearTree();
		void CopyTree(const BPTree<T>& other);

		Iterator<T> Search(const T& entry) const;
		Iterator<T> LowerBound(const T& entry) const;
		Iterator<T> UpperBound(const T& entry) const;

		size_t Size();
		bool Empty() const;

		void PrintAsTree(std::ostream& out = std::cout, int level = 0) const;

		BPTree<T>* GetSmallestNode() const;
		BPTree<T>* GetLargestNode() const;

		void PrintAsLinked(std::ostream& out = std::cout) const;
		void PrintAsList(std::ostream& out = std::cout) const;

		friend std::ostream& operator<<(std::ostream& out, const BPTree<T>& tree) {
			tree.PrintAsTree(out);
			return out;
		}

		bool IsValid();

		Iterator<T> begin() const;
		Iterator<T> end() const;

		T& GetItem(size_t entry) {
			return data_[entry];
		}

		BPTree<T>* GetNext() {
			return next_;
		}

	private:
		static const int kMaximum = 2;
		static const int kMinimum = kMaximum/2;

		size_t size_ = 0;
		size_t subset_size_ = 0;

		static const size_t kDataCapacity = kMaximum + 1;
		static const size_t kSubsetCapacity = kMaximum + 2;

		util::Array<T> data_ = util::Array<T>(kDataCapacity);
		util::Array<BPTree*> subset_ = util::Array<BPTree*>(kSubsetCapacity);

		bool duplicates_allowed_;

		bool IsLeaf() const {
			return subset_size_ == 0;
		}

		void LooseInsert(const T& entry);
		void FixExcess(int child);

		void LooseRemove(const T& entry, BPTree<T>* origin = nullptr, size_t offset = 0);
		void FixShortage(int child);

		T RemoveBiggest();

		void RotateLeft(int child);
		void RotateRight(int child);
		void MergeWithNextSubset(size_t subset);
		void EraseNode();

		void TransferLeft(size_t subset);
		void TransferRight(size_t subset);

		const T& Get(const T& entry) const;
		T& Get(const T& entry);
		T* Find(const T& entry);

		bool Contains(const T& entry) const;

		BPTree<T>* LinkLeaves(BPTree<T>* previous = nullptr);

		BPTree<T>* next_ = nullptr;
	};

	template<typename T>
	BPTree<T>::BPTree(bool duplicates_allowed) : duplicates_allowed_(duplicates_allowed)
	{}

	template<typename T>
	BPTree<T>::BPTree(const BPTree<T>& other)
	{
		CopyTree(other);
		LinkLeaves();
	}

	template<typename T>
	BPTree<T>& BPTree<T>::operator=(const BPTree<T>& other)
	{
		if(this == &other)
			return *this;
		ClearTree();
		CopyTree(other);
		LinkLeaves();
		return *this;
	}

	template<typename T>
	BPTree<T>::~BPTree<T>()
	{
		ClearTree();
	}

	template<typename T>
	void BPTree<T>::ClearTree()
	{
		for(size_t i = 0; i < subset_size_; ++i) {
			if(subset_[i] != nullptr) {
				subset_[i]->ClearTree();
				delete subset_[i];
				subset_[i] = nullptr;
			}
		}
		EraseNode();
	}

	template<typename T>
	void BPTree<T>::CopyTree(const BPTree<T>& other)
	{
		util::CopyArray(other.data_, data_);
		duplicates_allowed_ = other.duplicates_allowed_;
		subset_size_ = other.subset_size_;
		for(size_t i = 0; i < other.subset_size_; ++i) {
			if(other.subset_[i] != nullptr) {
				subset_[i] = new BPTree<T>(other.duplicates_allowed_);
				subset_[i]->CopyTree(*(other.subset_[i]));
			}
		}
	}

	template<typename T>
	void BPTree<T>::PrintAsTree(std::ostream& out, int level) const
	{
		if(subset_size_ > 1) {
			for(size_t i = subset_size_ - 1; i >= subset_size_ / 2; --i) {
				if(subset_[i] != nullptr)
					subset_[i]->PrintAsTree(out, level+1);
				else {
					util::PrintArray(data_, 0, level+1, out);
					out << std::endl;
				}
			}
		}

		util::PrintArray(data_, size_, level, out);
		out << std::endl;

		if(subset_size_ > 1) {
			for(int i = (subset_size_/2) - 1; i >= 0; --i) {
				if(subset_[i] != nullptr)
					subset_[i]->PrintAsTree(out, level+1);
				else {
					util::PrintArray(data_, 0, level+1, out);
					out << std::endl;
				}
			}
		}
	}

	template<typename T>
	void BPTree<T>::LooseInsert(const T& entry)
	{
		T* item = Find(entry);

		if(item != nullptr) {
			if(!duplicates_allowed_)
				*item = entry;
			else
				*item += entry;
			return;
		}

		auto index = util::IndexOfFirstGreater(data_, entry);
		if(!IsLeaf() && data_[index] != entry)
			subset_[index]->LooseInsert(entry);
		else {
			if (data_[index] != entry)
				util::InsertItem(data_, index, entry);
			return;
		}

		if(subset_[index]->size_ > kMaximum)
			FixExcess(index);
	}

	template<typename T>
	void BPTree<T>::Insert(const T& entry)
	{
		LooseInsert(entry);

		if(size_ > kMaximum) {
			auto temp = new BPTree<T>(duplicates_allowed_);

			util::CopyArray(data_, temp->data_);
			util::CopyArray(subset_, temp->subset_);

			EraseNode();

			subset_[0] = temp;
			subset_size_ = 1;

			FixExcess(0);
		}
	}

	template<typename T>
	void BPTree<T>::FixExcess(int child)
	{
		BPTree<T>* node = subset_[child];

		if(node->size_ <= kMaximum)
			return;

		auto split = new BPTree<T>(duplicates_allowed_);

		util::Split(node->data_, split->data_);
		util::Split(node->subset_, split->subset_);

		split->next_ = node->next_;
		node->next_ = split;

		T item = node->data_.PopBack();
		util::OrderedInsert(data_, item);
		util::InsertItem(subset_, child+1, split);

		if(split->IsLeaf())
			util::OrderedInsert(split->data_, item);
	}

	template<typename T>
	const T& BPTree<T>::Get(const T& entry) const {
		size_t index = util::IndexOfFirstGreater(data_, size_, entry);
		bool found = index < size_ && data_[index] == entry;

		if(found && IsLeaf())
			return data_[index];
		else if(found && !IsLeaf())
			return subset_[index+1]->Find(entry);
		else if (!found && !IsLeaf())
			return subset_[index]->Find(entry);
		else if(!found && IsLeaf())
			throw std::out_of_range("Item not in tree");
	}

	template<typename T>
	T& BPTree<T>::Get(const T& entry) {
		T* item = Find(entry);
		if(item == nullptr)
			Insert(entry);
		return *Find(entry);
	}

	template<typename T>
	T* BPTree<T>::Find(const T& entry) {
		size_t index = util::IndexOfFirstGreater(data_, entry);
		bool found = index < size_ && data_[index] == entry;

		if(found && IsLeaf())
			return &data_[index];
		else if(found && !IsLeaf())
			return subset_[index+1]->Find(entry);
		else if(!found && !IsLeaf())
			return subset_[index]->Find(entry);
		else if(!found && IsLeaf())
			return nullptr;
		return nullptr;
	}

	template<typename T>
	bool BPTree<T>::Contains(const T& entry) const
	{
		return data_[util::IndexOfFirstGreater(data_, size_, entry)] == entry;
	}

	template<typename T>
	void BPTree<T>::RotateLeft(int child)
	{
		if(subset_[child] == nullptr) {
			subset_[child] = new BPTree<T>(duplicates_allowed_);
			subset_size_++;
		}

		util::AppendItem(subset_[child]->data_, data_[child]);

		if(subset_[child+1] == nullptr)
			throw std::runtime_error("BPTree::RotateLeft: Can't AppendItem");
		data_[child] = util::DeleteItem(subset_[child+1]->data_, 0);

		if(subset_[child+1]->subset_size_ > 0) {
			if(subset_[child+1]->subset_[0] != nullptr) {
				util::AppendItem(subset_[child]->subset_,
					util::DeleteItem(subset_[child+1]->subset_, 0));
			}
		}

	}

	template<typename T>
	void BPTree<T>::RotateRight(int child)
	{
		if(subset_[child+1] == nullptr) {
			subset_[child+1] = new BPTree<T>(duplicates_allowed_);
			subset_size_++;
		}

		util::InsertItem(subset_[child+1]->data_, 0, data_[child]);

		if(subset_[child] == nullptr)
			throw std::runtime_error("BPTree::RotateRight: Can't InsertItem");
		data_[child] = subset_[child]->data_.PopBack();

		if(subset_[child]->subset_size_ > 0) {
			if(subset_[child]->subset_[subset_[child]->subset_size_ - 1] != nullptr) {
				util::InsertItem(subset_[child+1]->subset_, 0,
					subset_[child]->subset_.PopBack());
			}
		}
	}

	template<typename T>
	void BPTree<T>::EraseNode()
	{
		size_ = 0;
		next_ = nullptr;
		subset_size_ = 0;

		for(size_t i = 0; i < kSubsetCapacity; ++i) {
			if(i < kDataCapacity)
				data_[i] = T();
			subset_[i] = nullptr;
		}
	}

	template<typename T>
	void BPTree<T>::MergeWithNextSubset(size_t subset)
	{
		if(subset + 1 >= subset_size_)
			throw std::out_of_range("BPTree::MergeWithNextSubset");

		util::AttachArray(subset_[subset]->data_, subset_[subset + 1]->data_);
		util::AttachArray(subset_[subset]->subset_, subset_[subset+1]->subset_);

		subset_[subset]->next_ = subset_[subset+1]->next_;
		subset_[subset+1]->EraseNode();
		delete subset_[subset+1];
		util::DeleteItem(subset_, subset+1);
	}

	template<typename T>
	void BPTree<T>::Remove(const T& entry)
	{
		LooseRemove(entry);
		if(size_ < kMinimum && subset_size_ > 0) {
			auto shrink_ptr = subset_[0];
			subset_[0] = nullptr;
			util::CopyArray(shrink_ptr->data_, data_);
			util::CopyArray(shrink_ptr->subset_, subset_);

			shrink_ptr->EraseNode();
			delete shrink_ptr;
		}
	}

	template<typename T>
	void BPTree<T>::LooseRemove(const T& entry, BPTree<T>* origin, size_t offset)
	{
		size_t index = util::IndexOfFirstGreater(data_, entry);
		bool found = index < size_ && data_[index] == entry;

		if(found && IsLeaf()) {
			util::DeleteItem(data_, index);
			if(origin != nullptr) {
				if(size_ > 0)
					origin->data_[offset] = data_[0];
				else {
					if (next_ != nullptr)
						origin->data_[offset] = next_->data_[0];
				}
			}
		}
		else if (found && !IsLeaf()) {
			subset_[index + 1]->LooseRemove(entry, this, index);
			if(index == size_)
				index--;
			FixShortage(index);
		}
		else if(!found && !IsLeaf()) {
			subset_[index]->LooseRemove(entry, origin, offset);
			if(index == size_)
				index--;
			FixShortage(index);
		}
		else if (!found && IsLeaf()) {
			throw std::out_of_range("BPTree::LooseRemove: Item not in tree");
		}

		for(size_t i = 0; i < size_; ++i)
			FixShortage(i);
	}

	template<typename T>
	T BPTree<T>::RemoveBiggest()
	{
		if(IsLeaf())
			return data_.PopBack();

		T item = subset_[subset_size_ - 1]->RemoveBiggest();

		if(subset_[subset_size_ - 1]->size_ <= 0)
			delete subset_.PopBack();

		FixShortage(size_ - 1);
		return item;
	}

	template<typename T>
	void BPTree<T>::FixShortage(int child)
	{
		BPTree<T>* left = subset_[child];
		BPTree<T>* right = subset_[child+1];

		if(IsLeaf())
			return;

		if(left != nullptr && right != nullptr)
			if(left->size_ >= kMinimum && right->size_ >= kMinimum)
				return;

		if(!left->IsLeaf() || !right->IsLeaf()) {
			if(left != nullptr && left->size_ > kMinimum &&
				(right == nullptr || right->size_ <= kMinimum))
				RotateRight(child);
			else if(right != nullptr && right->size_ > kMinimum &&
				(left == nullptr || left->size_ <= kMinimum))
				RotateLeft(child);
			else {
				T item = util::DeleteItem(data_, child);
				if(right == nullptr) {
					auto temp = new BPTree<T>(duplicates_allowed_);
					util::InsertItem(subset_, child+1, temp);
					right = subset_[child+1];
				}
				util::OrderedInsert(right->data_, item);

				MergeWithNextSubset(child);
			}
		}
		else {
			if(left != nullptr && left->size_ > kMinimum &&
				(right == nullptr || right->size_ <= kMinimum))
				TransferRight(child);
			else if(right != nullptr && right->size_ > kMinimum
				&& (left == nullptr || left->size_ <= kMinimum))
				TransferLeft(child);
			else {
				util::DeleteItem(data_, child);
				if(right == nullptr) {
					auto temp = new BPTree<T>(duplicates_allowed_);
					util::InsertItem(subset_, child+1, temp);
					right = subset_[child+1];
				}

				MergeWithNextSubset(child);
			}
		}
	}

	template<typename T>
	BPTree<T>* BPTree<T>::GetSmallestNode() const
	{
		BPTree<T>* smallest = (BPTree<T>*)this;
		while(!smallest->IsLeaf())
			smallest = smallest->subset_[0];

		return smallest;
	}

	template<typename T>
	BPTree<T>* BPTree<T>::GetLargestNode() const {
		BPTree<T>* largest = (BPTree<T>*)this;
		while(!largest->IsLeaf())
			largest = largest->subset_[largest->subset_size_ - 1];

		return largest;
	}

	template<typename T>
	void BPTree<T>::PrintAsLinked(std::ostream& out) const
	{
		BPTree<T>* curr_node = GetSmallestNode();
		while(curr_node != nullptr) {
			util::PrintArray(curr_node->data_,0, out);
			out << " -> ";
			curr_node = curr_node->next_;
		}
		out << " |||";
	}

	template<typename T>
	void BPTree<T>::PrintAsList(std::ostream& out) const
	{
		for(auto it = begin(); it != nullptr; ++it) {
			if(it != nullptr)
				out << *it << "\n";
		}
	}

	template<typename T>
	Iterator<T> BPTree<T>::begin() const
	{
		auto node = GetSmallestNode();
		if(node->size_ == 0)
			return Iterator<T>(nullptr, 0);
		return Iterator<T>(node);
	}

	template<typename T>
	Iterator<T> BPTree<T>::end() const
	{
		auto node = GetLargestNode();
		if(node->size_ == 0)
			return Iterator<T>(nullptr);
		return Iterator<T>(node);
	}

	template<typename T>
	Iterator<T> BPTree<T>::Search(const T& entry) const
	{
		size_t index = util::IndexOfFirstGreater(data_, entry);
		bool found = index < size_ && data_[index] == entry;

		if(found && IsLeaf())
			return Iterator<T>((BPTree<T>*)this, index);
		else if(found && !IsLeaf())
			return subset_[index+1]->Search(entry);
		else if(!found && !IsLeaf())
			return subset_[index]->Search(entry);
		else if(!found && IsLeaf())
			return Iterator<T>(nullptr);
		return Iterator<T>(nullptr);
	}

	template<typename T>
	void BPTree<T>::TransferLeft(size_t subset)
	{
		if(subset >= subset_size_)
			throw std::out_of_range("BPTree::TransferLeft");

		if(subset_[subset+1]->size_ > 1)
			throw std::runtime_error("BPTree::TransferLeft: Sibling has nothing to borrow");

		if(subset_[subset]->subset_size_ != 0)
			throw std::runtime_error("BPTree::TransferLeft: Call a non leaf");

		util::AppendItem(subset_[subset]->data_,
			util::DeleteItem(subset_[subset+1]->data_, 0));

		data_[subset] = subset_[subset + 1]->data_[0];
	}

	template<typename T>
	void BPTree<T>::TransferRight(size_t subset)
	{
		if(subset_[subset+1]->size_ > 1)
			throw std::runtime_error("BPTree::TransferRight: Sibling has nothing to borrow");

		if(subset_[subset]->subset_size_ != 0)
			throw std::runtime_error("BPTree::TransferRight: Call a non leaf");

		util::InsertItem(subset_[subset+1]->data_, 0,
			subset_[subset+1]->data_.PopBack());

		data_[subset] = subset_[subset+1]->data_[0];
	}

	template<typename T>
	BPTree<T>* BPTree<T>::LinkLeaves(BPTree<T>* previous)
	{
		if(IsLeaf())
			return nullptr;

		if(subset_size_ > 0 && subset_[0]->IsLeaf()) {
			if(previous != nullptr)
				previous->next_ = subset_[0];
			for(size_t i = 0;i < subset_size_ - 1; ++i)
				subset_[i]->next_ = subset_[i+1];
			return subset_[subset_size_ - 1];
		}
		else {
			BPTree<T>* last_link = previous;
			for(size_t i = 0; i < subset_size_ - 1; ++i) {
				BPTree<T>* left = subset_[i];
				BPTree<T>* right = subset_[i+1];
				last_link = left->LinkLeaves(last_link);
				BPTree<T>* right_link = right->LinkLeaves(last_link);
				last_link = right_link;
			}
			return last_link;
		}
	}

	template<typename T>
	bool BPTree<T>::IsValid()
	{
		if(IsLeaf())
			return true;
		if(!util::IsLess(subset_[subset_size_-1]->data_, data_[size_ - 1]))
			return false;

		for(size_t i = 0; i < size_; ++i) {
			if(!util::IsGreater(subset_[i]->data_, data_[size_ - 1]))
				return false;
			if(Find(data_[i]) == nullptr)
				return false;
		}

		for(size_t i = 0; i < subset_size_; ++i)
			if(!subset_[i]->IsValid())
				return false;
		return true;
	}

	template<typename T>
	size_t BPTree<T>::Size()
	{
		size_t s = 0;
		for(auto it = begin(); it != nullptr; ++it)
			++s;
		return s;
	}

	template<typename T>
	bool BPTree<T>::Empty() const
	{
		return begin() == nullptr;
	}

	template<typename T>
	Iterator<T> BPTree<T>::LowerBound(const T& entry) const
	{
		size_t index = util::IndexOfFirstGreater(data_, entry);
		bool found = index < size_ && data_[index] == entry;

		if(found && IsLeaf())
			return Iterator<T>((BPTree<T>*)this, index);
		else if(found && !IsLeaf())
			return subset_[index+1]->LowerBound(entry);
		else if(!found && !IsLeaf())
			return subset_[index]->LowerBound(entry);
		else if(!found && IsLeaf()) {
			if(index < size_)
				return Iterator<T>((BPTree<T>*)this, index);
			else
				return Iterator<T>(next_);
		}

		return Iterator<T>(nullptr);
	}

	template<typename T>
	Iterator<T> BPTree<T>::UpperBound(const T& entry) const
	{
		size_t index = util::IndexOfFirstGreater(data_, entry);
		bool found = index < size_ && data_[index] == entry;

		if(found && IsLeaf()) {
			Iterator<T> lower((BPTree<T>*)this, index);
			return ++lower;
		}
		else if(found && !IsLeaf())
			return subset_[index+1]->UpperBound(entry);
		else if(!found && !IsLeaf())
			return subset_[index]->UpperBound(entry);
		else if(!found && IsLeaf()) {
			if(index <= size_ - 1)
				return Iterator<T>((BPTree<T>*)this, index);
			else
				return Iterator<T>(next_, 0);
		}

		return Iterator<T>(nullptr);
	}
}