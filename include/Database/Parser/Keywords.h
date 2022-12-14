#pragma once
#include "../Constants.h"

namespace sql_parser {

	enum Keyword {
		ZERO,
		SELECT,
		INSERT,
		CREATE,
		STRING,
		INTO,
		FROM,
		WHERE,
		TABLE,
		VALUES,
		DROP,
		LIST_TABLES,
		EXEC,
		EXIT,
		ASTERISK,
		COMMA,
		OPEN_PARENTH,
		CLOSE_PARENTH,
		QUOTE,
		RELATIONAL_OPERATOR,
		LOGICAL_OPERATOR
		#ifdef ENABLE_NON_STANDARD_SQL
		,FIELDS
		#endif
	};
}