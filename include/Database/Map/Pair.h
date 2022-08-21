#pragma once
#include <iostream>

namespace Map {

	template<typename K, typename V>
	struct Pair {
		K key_;
		V value_;
		explicit Pair(const K& key = K(), const V& value = V())
			:key_(key), value_(value)
		{}
		void operator+=(const Pair& other);
	};
	template<typename K, typename V>
	void Pair<K, V>::operator+=(const Pair& other)
	{
		value_ += static_cast<V>(other.value_);
	}

	template<typename K, typename V>
	std::ostream& operator<<(std::ostream& out, const Pair<K, V>& pair) {
		out << "{ " << pair.key_ << ": " << pair.value_ << " }";
		return out;
	}

	template<typename K, typename V>
	bool operator==(const Pair<K, V>& left, const Pair<K, V>& right) {
		return left.key_ == right.key_;
	}

	template<typename K, typename V>
	bool operator!=(const Pair<K, V>& left, const Pair<K, V>& right) {
		return left.key_ != right.key_;
	}

	template<typename K, typename V>
	bool operator<(const Pair<K, V>& left, const Pair<K, V>& right) {
		return left.key_ < right.key_;
	}

	template<typename K, typename V>
	bool operator>(const Pair<K, V>& left, const Pair<K, V>& right) {
		return left.key_ > right.key_;
	}

	template<typename K, typename V>
	bool operator<=(const Pair<K, V>& left, const Pair<K, V>& right) {
		return left.key_ <= right.key_;
	}

	template<typename K, typename V>
	bool operator>=(const Pair<K, V>& left, const Pair<K, V>& right) {
		return left.key_ >= right.key_;
	}
}