#include <Database/Parser/SQLStateMachine.h>

sql_parser::SQLStateMachine::SQLStateMachine() {
	keywords_["SELECT"] = sql_parser::SELECT;
	keywords_["Insert"] = sql_parser::INSERT;
	keywords_["CREATE"] = sql_parser::CREATE;
	keywords_["INTO"] = sql_parser::INTO;
	keywords_["FROM"] = sql_parser::FROM;
	keywords_["WHERE"] = sql_parser::WHERE;
	keywords_["TABLE"] = sql_parser::TABLE;
	keywords_["VALUES"] = sql_parser::VALUES;
	keywords_["DROP"] = sql_parser::DROP;
	keywords_["TABLES"] = sql_parser::LIST_TABLES;
	keywords_["EXIT"] = sql_parser::EXIT;
	keywords_["EXEC"] = sql_parser::EXEC;
	keywords_["*"] = sql_parser::ASTERISK;
	keywords_[","] = sql_parser::COMMA;
	keywords_["("] = sql_parser::OPEN_PARENTH;
	keywords_[")"] = sql_parser::CLOSE_PARENTH;
	keywords_["="] = sql_parser::RELATIONAL_OPERATOR;
	keywords_[">"] = sql_parser::RELATIONAL_OPERATOR;
	keywords_["<"] = sql_parser::RELATIONAL_OPERATOR;
	keywords_["<="] = sql_parser::RELATIONAL_OPERATOR;
	keywords_[">="] = sql_parser::RELATIONAL_OPERATOR;
	keywords_["AND"] = sql_parser::LOGICAL_OPERATOR;
	keywords_["OR"] = sql_parser::LOGICAL_OPERATOR;
	keywords_["\""] = sql_parser::QUOTE;
#ifdef ENABLE_NON_STANDARD_SQL
	keywords_["MAKE"] = sql_parser::CREATE;
	keywords_["FIELDS"] = sql_parser::FIELDS;
#endif
	
	state_machine::InitTable(state_table_);
	state_machine::MarkFail(state_table_, 0);
	state_machine::MarkFail(state_table_, 1);
	state_machine::MarkFail(state_table_, 2);
	state_machine::MarkFail(state_table_, 3);
	state_machine::MarkFail(state_table_, 4);
	state_machine::MarkFail(state_table_, 5);
	state_machine::MarkSuccess(state_table_, 6);
	state_machine::MarkFail(state_table_, 7);
	state_machine::MarkFail(state_table_, 8);
	state_machine::MarkFail(state_table_, 9);
	state_machine::MarkFail(state_table_, 10);
	state_machine::MarkSuccess(state_table_, 11);
	state_machine::MarkFail(state_table_, 12);
	state_machine::MarkFail(state_table_, 13);
	state_machine::MarkFail(state_table_, 14);
	state_machine::MarkFail(state_table_, 15);
	state_machine::MarkFail(state_table_, 16);
	state_machine::MarkFail(state_table_, 17);
	state_machine::MarkFail(state_table_, 18);
	state_machine::MarkSuccess(state_table_, 19);
	state_machine::MarkFail(state_table_, 20);
	state_machine::MarkFail(state_table_, 21);
	state_machine::MarkFail(state_table_, 22);
	state_machine::MarkFail(state_table_, 23);
	state_machine::MarkFail(state_table_, 24);
	state_machine::MarkFail(state_table_, 25);
	state_machine::MarkFail(state_table_, 26);
	state_machine::MarkSuccess(state_table_, 27);
	state_machine::MarkFail(state_table_, 28);
	state_machine::MarkFail(state_table_, 29);
	state_machine::MarkFail(state_table_, 30);
	state_machine::MarkSuccess(state_table_, 31);
	state_machine::MarkSuccess(state_table_, 32);
	state_machine::MarkFail(state_table_, 33);
	state_machine::MarkFail(state_table_, 34);
	state_machine::MarkFail(state_table_, 35);
	state_machine::MarkFail(state_table_, 36);
	state_machine::MarkFail(state_table_, 37);
	state_machine::MarkFail(state_table_, 38);
	state_machine::MarkFail(state_table_, 39);
	state_machine::MarkFail(state_table_, 40);
	state_machine::MarkFail(state_table_, 41);
	state_machine::MarkFail(state_table_, 42);
	state_machine::MarkFail(state_table_, 43);
	state_machine::MarkFail(state_table_, 44);
	state_machine::MarkFail(state_table_, 45);
	state_machine::MarkFail(state_table_, 46);
	state_machine::MarkFail(state_table_, 47);
	state_machine::MarkFail(state_table_, 48);
	state_machine::MarkFail(state_table_, 49);
	state_machine::MarkFail(state_table_, 50);
	state_machine::MarkSuccess(state_table_, 51);

	parse_states_[1] ="command";
	
	parse_states_[13] = "command";

	parse_states_[20] = "command";
	
	parse_states_[29] = "command";
	
	parse_states_[32] = "command";
	
	parse_states_[50] = "command";
	
	parse_states_[2] = "fields";
	
	parse_states_[3] = "fields";
	
	parse_states_[17] = "fields";
	
	parse_states_[25] = "fields";
	
	parse_states_[51] = "fields";
	
	parse_states_[6] = "table_name";

	parse_states_[15] = "table_name";
	
	parse_states_[22] = "table_name";
	
	parse_states_[31] = "table_name";
	
	parse_states_[9] = "where";
	
	parse_states_[10] = "where";
	
	parse_states_[11] = "where";
	
	parse_states_[12] = "where";
	
	quote_states_.Insert(33);
	quote_states_.Insert(34);
	quote_states_.Insert(35);
	quote_states_.Insert(36);
	quote_states_.Insert(37);
	quote_states_.Insert(38);
	quote_states_.Insert(39);
	quote_states_.Insert(40);
	quote_states_.Insert(41);
	quote_states_.Insert(42);
	quote_states_.Insert(43);
	quote_states_.Insert(44);
	quote_states_.Insert(45);
	quote_states_.Insert(46);
	quote_states_.Insert(47);
	quote_states_.Insert(48);
	quote_states_.Insert(49);
	
#ifdef ENABLE_NON_STANDARD_SQL
	
	state_machine::MarkSuccess(state_table_, 25);

	state_machine::MarkSuccess(state_table_, 17);
#endif
	
	state_machine::MarkCell(0, state_table_, sql_parser::SELECT, 1);

	state_machine::MarkCell(1, state_table_, sql_parser::ASTERISK, 2);

	state_machine::MarkCell(1, state_table_, sql_parser::STRING, 3);
	
	state_machine::MarkCell(4, state_table_, sql_parser::STRING, 3);
	
	state_machine::MarkCell(5, state_table_, sql_parser::STRING, 6);
	
	state_machine::MarkCell(8, state_table_, sql_parser::STRING, 9);
	
	state_machine::MarkCell(10, state_table_, sql_parser::STRING, 11);
	
	state_machine::MarkCell(12, state_table_, sql_parser::STRING, 9);

	state_machine::MarkCell(3, state_table_, sql_parser::COMMA, 4);

	state_machine::MarkCell(2, state_table_, sql_parser::FROM, 5);
	
	state_machine::MarkCell(3, state_table_, sql_parser::FROM, 5);

	state_machine::MarkCell(6, state_table_, sql_parser::WHERE, 8);

	state_machine::MarkCell(9, state_table_, sql_parser::RELATIONAL_OPERATOR, 10);
	
	state_machine::MarkCell(11, state_table_, sql_parser::LOGICAL_OPERATOR, 12);
	
	state_machine::MarkCell(5, state_table_, sql_parser::QUOTE, 33);

	state_machine::MarkAll(33, state_table_, 34);
	
	state_machine::UnmarkCell(33, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(34, state_table_, 34);
	
	state_machine::UnmarkCell(34, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(34, state_table_, sql_parser::QUOTE, 6);
	
	state_machine::MarkCell(8, state_table_, sql_parser::QUOTE, 35);
	
	state_machine::MarkCell(12, state_table_, sql_parser::QUOTE, 35);

	state_machine::MarkAll(35, state_table_, 36);
	
	state_machine::UnmarkCell(35, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(36, state_table_, 36);
	
	state_machine::UnmarkCell(36, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(36, state_table_, sql_parser::QUOTE, 9);


	state_machine::MarkCell(10, state_table_, sql_parser::QUOTE, 37);

	state_machine::MarkAll(37, state_table_, 38);
	
	state_machine::UnmarkCell(37, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(38, state_table_, 38);
	
	state_machine::UnmarkCell(38, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(38, state_table_, sql_parser::QUOTE, 11);
	
	state_machine::MarkCell(0, state_table_, sql_parser::CREATE, 13);

	state_machine::MarkCell(13, state_table_, sql_parser::TABLE, 14);

	state_machine::MarkCell(15, state_table_, sql_parser::OPEN_PARENTH, 16);
	
	state_machine::MarkCell(17, state_table_, sql_parser::CLOSE_PARENTH, 19);

	state_machine::MarkCell(14, state_table_, sql_parser::STRING, 15);
	
	state_machine::MarkCell(16, state_table_, sql_parser::STRING, 17);
	
	state_machine::MarkCell(18, state_table_, sql_parser::STRING, 17);

	state_machine::MarkCell(17, state_table_, sql_parser::COMMA, 18);
	
	state_machine::MarkCell(13, state_table_, sql_parser::QUOTE, 40);

	state_machine::MarkAll(40, state_table_, 41);
	
	state_machine::UnmarkCell(40, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(41, state_table_, 41);
	
	state_machine::UnmarkCell(41, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(41, state_table_, sql_parser::QUOTE, 15);

	state_machine::MarkCell(15, state_table_, sql_parser::QUOTE, 42);
	
	state_machine::MarkCell(16, state_table_, sql_parser::QUOTE, 42);
	
	state_machine::MarkCell(18, state_table_, sql_parser::QUOTE, 42);

	state_machine::MarkAll(42, state_table_, 43);
	
	state_machine::UnmarkCell(42, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(43, state_table_, 43);
	
	state_machine::UnmarkCell(43, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(43, state_table_, sql_parser::QUOTE, 17);
	
	state_machine::MarkCell(0, state_table_, sql_parser::INSERT, 20);

	state_machine::MarkCell(20, state_table_, sql_parser::INTO, 21);

	state_machine::MarkCell(21, state_table_, sql_parser::STRING, 22);
	
	state_machine::MarkCell(24, state_table_, sql_parser::STRING, 25);
	
	state_machine::MarkCell(26, state_table_, sql_parser::STRING, 25);
	
	state_machine::MarkCell(23, state_table_, sql_parser::OPEN_PARENTH, 24);
	
	state_machine::MarkCell(24, state_table_, sql_parser::CLOSE_PARENTH, 27);
	
	state_machine::MarkCell(25, state_table_, sql_parser::CLOSE_PARENTH, 27);

	state_machine::MarkCell(25, state_table_, sql_parser::COMMA, 26);

	state_machine::MarkCell(22, state_table_, sql_parser::VALUES, 23);
	
	state_machine::MarkCell(20, state_table_, sql_parser::QUOTE, 44);

	state_machine::MarkAll(44, state_table_, 45);
	
	state_machine::UnmarkCell(44, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(45, state_table_, 45);
	
	state_machine::UnmarkCell(45, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(45, state_table_, sql_parser::QUOTE, 22);
	
	state_machine::MarkCell(23, state_table_, sql_parser::QUOTE, 46);
	
	state_machine::MarkCell(24, state_table_, sql_parser::QUOTE, 46);
	
	state_machine::MarkCell(26, state_table_, sql_parser::QUOTE, 46);

	state_machine::MarkAll(46, state_table_, 47);
	
	state_machine::UnmarkCell(46, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(47, state_table_, 47);
	
	state_machine::UnmarkCell(47, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(47, state_table_, sql_parser::QUOTE, 25);
	
	state_machine::MarkCell(0, state_table_, sql_parser::DROP, 29);
	
	state_machine::MarkCell(29, state_table_, sql_parser::TABLE, 30);
	
	state_machine::MarkCell(30, state_table_, sql_parser::STRING, 31);

	state_machine::MarkCell(20, state_table_, sql_parser::QUOTE, 48);

	state_machine::MarkAll(48, state_table_, 49);
	
	state_machine::UnmarkCell(48, state_table_, sql_parser::QUOTE);

	state_machine::MarkAll(49, state_table_, 49);
	
	state_machine::UnmarkCell(49, state_table_, sql_parser::QUOTE);

	state_machine::MarkCell(49, state_table_, sql_parser::QUOTE, 31);
	
	state_machine::MarkCell(0, state_table_, sql_parser::LIST_TABLES, 32);
	
	state_machine::MarkCell(0, state_table_, sql_parser::EXIT, 32);
	
	state_machine::MarkCell(0, state_table_, sql_parser::EXEC, 50);
	
	state_machine::MarkAll(50, state_table_, 51);
	
	state_machine::MarkAll(51, state_table_, 51);

	#ifdef ENABLE_NON_STANDARD_SQL
		state_machine::MarkCell(23, state_table_, sql_parser::STRING, 25);
		state_machine::MarkCell(15, state_table_, sql_parser::FIELDS, 28);
		state_machine::MarkCell(28, state_table_, sql_parser::STRING, 17);
	#endif
}

int sql_parser::SQLStateMachine::UpdateState(const std::string& token) {
	sql_parser::Keyword type = sql_parser::STRING;

	std::string upper_token = string_util::UpperCase(token);
	if(keywords_.Contains(upper_token))
		type = keywords_[upper_token];
	last_state_ = state_table_[last_state_][type];
	return last_state_;
}

void sql_parser::SQLStateMachine::ResetState()
{
	last_state_ = 0;
}

bool sql_parser::SQLStateMachine::IsSuccess()
{
	return state_machine::IsSuccess(state_table_, last_state_);
}

bool sql_parser::SQLStateMachine::IsFail()
{
	return !state_machine::IsSuccess(state_table_, last_state_);
}

bool sql_parser::SQLStateMachine::IsInvalid()
{
	return last_state_ == -1;
}

bool sql_parser::SQLStateMachine::IsQuoteState(int state)
{
	return quote_states_.Search(state) != nullptr;
}

bool sql_parser::SQLStateMachine::IsParseState(int state)
{
	return parse_states_.Contains(state);
}

std::string sql_parser::SQLStateMachine::GetParseKey(int state)
{
	if(parse_states_.Contains(state))
		return parse_states_[state];
	else
		return "";
}