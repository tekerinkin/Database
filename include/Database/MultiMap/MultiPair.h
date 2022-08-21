#pragma once
#include "../util/Vector_util.h"
#include <iostream>
#include <vector>

namespace multi_map {

	template<typename K, typename V>
	struct Pair {
		K key_;
		std::vector<V> values_;
		explicit Pair(const K& key = K())
			:key_(key)
		{}

		Pair(const K& key, const V& value) : key_(key)
		{
			values_.push_back(value);
		}

		Pair(const K& key, const std::vector<V>& values) : key_(key)
		{
			this->values_ = values;
		}

		void operator+=(const Pair& other) {
			for(size_t i = 0; i < other.values_.size(); ++i) {
				values_.push_back(other.values_.at(i));
			}
		}

		void operator+=(const V& item) {
			values_.push_back(item);
		}
	};

	template<typename K, typename V>
	std::ostream& operator<<(std::ostream& out, const Pair<K, V>& pair) {
		out << "{ " << pair.key_ << ": [ ";
		for(size_t i = 0; i < pair.values_.size(); i++) {
			out << pair.values_[i];
			if(i < pair.values_.size() - 1)
				out << ",";
			out << " ";
		}
		out << "] }";
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
