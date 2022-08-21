#pragma once
#include <Database/B+tree.h>
#include <Database/Map/Pair.h>

namespace Map {

	template<typename K, typename V>
	class Map;

	template<typename K, typename V>
	class Iterator {
	public:
		friend class Map<K, V>;

		explicit Iterator(db::Iterator<Pair<K, V>> it) : tree_iter_(it)
		{}

		const V& operator*(){
			return tree_iter_->value_;
		}

		V* operator->() {
			return &tree_iter_.operator->()->value_;
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
	class Map {
	public:
		Map();

		V Get(const K& key) const;
		V& At(const K& key);
		V& operator[](const K& key);
		const V& operator[](const K& key) const;
		const V& At(const K& key) const;

		void Set(const K& key, const V& value);
		void Erase(const K& key);
		void Clear() {
			tree_.ClearTree();
		}

		Iterator<K, V> begin() const;
		Iterator<K, V> end() const;
		Iterator<K, V> LowerBound(const K& entry) const;
		Iterator<K, V> UpperBound(const K& entry) const;

		bool Contains(const K& key, const V& value);
		bool Contains(const K& key);

		friend std::ostream& operator<<(std::ostream& out, const Map<K, V>& map) {
			map.tree_.PrintAsList();
			return out;
		}


	private:
		db::BPTree<Pair<K, V>> tree_;
	};

	template<typename K, typename V>
	Map<K, V>::Map() {}

	template<typename K, typename V>
	void Map<K, V>::Set(const K& key, const V& value) {
		tree_.Insert(Pair<K, V>(key, value));
	}

	template<typename K, typename V>
	V& Map<K, V>::operator[](const K& key)
	{
		if(tree_.Search(Pair<K, V>(key, V())) == nullptr)
			tree_.Insert(Pair<K, V>(key, V()));
		return tree_.Search(Pair<K, V>(key, V()))->value_;
	}

	template<typename K, typename V>
	const V& Map<K, V>::operator[](const K& key) const {
		return tree_.Search(Pair<K, V>(key, V()))->value_;
	}

	template<typename K, typename V>
	V& Map<K, V>::At(const K& key) {
		return tree_.Search(Pair<K, V>(key, V()))->value_;
	}

	template<typename K, typename V>
	V Map<K, V>::Get(const K& key) const{
		return tree_.Search(Pair<K, V>(key, V()))->value_;
	}

	template<typename K, typename V>
	const V& Map<K, V>::At(const K& key) const {
		try{
			return tree_.Search(key);
		} catch(std::out_of_range()) {
			throw std::out_of_range("Map::At: Key not found");
		}
	}

	template<typename K, typename V>
	void Map<K, V>::Erase(const K& key)
	{
		tree_.Remove(Pair<K, V>(key, V()));
	}

	template<typename K, typename V>
	bool Map<K, V>::Contains(const K& key)
	{
		auto search = tree_.Search(Pair<K, V>(key, V()));
		return search != nullptr;
	}

	template<typename K, typename V>
	bool Map<K,V>::Contains(const K& key, const V& value)
	{
		auto search = tree_.Search(key);
		return search != nullptr && search->value_ == value;
	}

	template<typename K, typename V>
	Iterator<K, V> Map<K, V>::begin() const
	{
		return Iterator<K, V>(tree_.begin());
	}

	template<typename K, typename V>
	Iterator<K, V> Map<K, V>::end() const
	{
		return Iterator<K, V>(tree_.end());
	}

	template<typename K, typename V>
	Iterator<K, V> Map<K, V>::LowerBound(const K& entry) const
	{
		return Iterator<K, V>(tree_.LowerBound(entry));
	}

	template<typename K, typename V>
	Iterator<K, V> Map<K, V>::UpperBound(const K& entry) const
	{
		return Iterator<K, V>(tree_.UpperBound(entry));
	}
}