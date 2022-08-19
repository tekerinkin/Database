#pragma once
#include "../B+tree.h"
#include "MultiPair.h"

namespace multi_map {
	template<typename K, typename V>
	class MultiMap;

	template<typename K, typename V>
	class Iterator {
	public:
		friend class MultiMap<K, V>;

		explicit Iterator(db::Iterator<Pair<K, V>> it) : tree_iter_(it)
		{}

		std::vector<V>& operator*(){
			return tree_iter_->values_;
		}

		std::vector<V>* operator->() {
			return &tree_iter_.operator->()->values_;
		}

		Iterator operator++(int) {
			Iterator temp = tree_iter_;
			tree_iter_++;
			return temp;
		}

		Iterator operator++() {
			tree_iter_++;
			return *this;
		}

		friend bool operator==(const Iterator& left, const Iterator& right) {
			return left.tree_iter_ == right.tree_iter_;
		}

		friend bool operator!=(const Iterator& left, const Iterator& right) {
			return left.tree_iter_ != right.tree_iter_;
		}

		friend bool operator==(const Iterator& left, const V& right) {
			return *(left.tree_iter_) == right;
		}

		friend bool operator!=(const Iterator& left, const V& right) {
			return *(left.tree_iter_) != right;
		}

		bool IsNull() {
			return tree_iter_ == nullptr;
		}

		K Key() {
			return tree_iter_->key_;
		}

	private:
		db::Iterator<Pair<K, V>> tree_iter_;
	};

	template<typename K, typename V>
	class MultiMap {
	public:
		MultiMap();

		std::vector<V>& Get(const K& key) const;
		std::vector<V>& At(const K& key);
		std::vector<V>& operator[](const K& key);
		const std::vector<V>& operator[](const K& key) const;
		const std::vector<V>& At(const K& key) const;

		void Insert(const K& key, const V& value);
		void Erase(const K& key);
		void Clear() {
			tree_.ClearTree();
		}

		Iterator<K, V> begin() const;
		Iterator<K, V> end() const;
		Iterator<K, V> LowerBound(const K& entry) const;
		Iterator<K, V> UpperBound(const K& entry) const;

		bool Contains(const K& key);

		friend std::ostream& operator<<(std::ostream& out, const MultiMap<K, V>& map) {
			map.tree_.PrintAsList();
			return out;
		}


	private:
		db::BPTree<Pair<K, V>> tree_;
	};

	template<typename K, typename V>
	MultiMap<K, V>::MultiMap() : tree_(true) {}

	template<typename K, typename V>
	void MultiMap<K, V>::Insert(const K& key, const V& value) {
		tree_.Insert(Pair<K, V>(key, value));
	}

	template<typename K, typename V>
	std::vector<V>& MultiMap<K, V>::operator[](const K& key)
	{
		return At(key);
	}

	template<typename K, typename V>
	const std::vector<V>& MultiMap<K, V>::operator[](const K& key) const {
		try {
			return tree_.Search(key);
		} catch(std::out_of_range()) {
			throw std::out_of_range("Key not found");
		}
	}

	template<typename K, typename V>
	std::vector<V>& MultiMap<K, V>::At(const K& key) {
		if(tree_.Search(Pair<K, V>(key, V())) == nullptr)
			tree_.Insert(Pair<K, V>(key));
		return tree_.Search(Pair<K, V>(key, V()))->values_;
	}

	template<typename K, typename V>
	std::vector<V>& MultiMap<K, V>::Get(const K& key) const{
		return tree_.Search(Pair<K, V>(key, V()))->values_;
	}

	template<typename K, typename V>
	const std::vector<V>& MultiMap<K, V>::At(const K& key) const {
		try{
			return tree_.Search(key);
		} catch(std::out_of_range()) {
			throw std::out_of_range("MultiMap::At: Key not found");
		}
	}

	template<typename K, typename V>
	void MultiMap<K, V>::Erase(const K& key)
	{
		tree_.Remove(key);
	}

	template<typename K, typename V>
	bool MultiMap<K, V>::Contains(const K& key)
	{
		auto search = tree_.Search(key);
		return search != nullptr;
	}

	template<typename K, typename V>
	Iterator<K, V> MultiMap<K, V>::begin() const
	{
		return Iterator<K, V>(tree_.begin());
	}

	template<typename K, typename V>
	Iterator<K, V> MultiMap<K, V>::end() const
	{
		return Iterator<K, V>(tree_.end());
	}

	template<typename K, typename V>
	Iterator<K, V> MultiMap<K, V>::LowerBound(const K& entry) const
	{
		return Iterator<K, V>(tree_.LowerBound(entry));
	}

	template<typename K, typename V>
	Iterator<K, V> MultiMap<K, V>::UpperBound(const K& entry) const
	{
		return Iterator<K, V>(tree_.UpperBound(entry));
	}
}