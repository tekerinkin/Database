#pragma once
#include "../Map/Map.h"
#include "Keywords.h"
#include "../StringTokenizer/Token.h"
#include "../StringTokenizer/StateTable.h"
#include "../Constants.h"
#include "../util/String_util.h"
#include <cctype>
#include <string>

namespace sql_parser {

	class SQLStateMachine {
	public:
		SQLStateMachine();

		int UpdateState(const std::string& token);

		int GetState() {
			return last_state_;
		}

		void ResetState();

		bool IsSuccess();

		bool IsFail();

		bool IsInvalid();

		bool IsQuoteState(int state);

		bool IsParseState(int state);

		std::string GetParseKey(int state);

	private:
		Map::Map<std::string, Keyword> keywords_;
		int state_table_[state_machine::kNumRows][state_machine::kMaxCols];
		int last_state_ = 0;
		Map::Map<int, std::string> parse_states_;
		db::BPTree<int> quote_states_;
	};
}