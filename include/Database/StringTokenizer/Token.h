#pragma once
#include <iostream>
#include <string>

namespace string_tokenizer {
	class Token {
	public:
		Token();
		Token(const std::string& str, int type);
		int Type() const {
			return type_;
		}

		std::string TypeString() const;
		std::string TokenStr() const {
			return token_;
		}

		void SetString(const std::string& str) {
			token_ = str;
		}

		void SetType(int type) {
			type_ = type;
		}

		friend std::ostream& operator<<(std::ostream& out, const Token& token) {
			return out << token.token_;
		}

		friend bool operator !=(const Token& left, const Token& right) {
			return left.token_ != right.token_;
		}

		friend bool operator !=(const Token& left, const std::string& right) {
			return left.token_ != right;
		}

		friend bool operator ==(const Token& left, const Token& right) {
			return left.token_ == right.token_;
		}

		friend bool operator ==(const Token& left, const std::string& right) {
			return left.token_ != right;
		}

	private:
		std::string token_;
		int type_;
	};
}