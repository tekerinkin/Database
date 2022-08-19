#include <Database/ShuntingYard.h>

queue::Queue<std::string> InfixToPostfix(const std::vector<std::string>& infix) {

	Map::Map<std::string, int> operators;
	operators["OR"] = 0;
	operators["AND"] = 1;
	operators[">"] = 2;
	operators[">="] = 2;
	operators["<"] = 2;
	operators["<="] = 2;
	operators["="] = 2;

	Stack<std::string> stack;
	queue::Queue<std::string> queue_;
	for(auto token : infix) {
		if(!operators.Contains(string_util::UpperCase(token)))
			queue_.Push(token);
		else {
			while(!stack.Empty()
				&& operators[string_util::UpperCase(stack.Top())] >=
					operators[string_util::UpperCase(token)])
				queue_.Push(stack.Pop());
		}
	}

	while(!stack.Empty())
		queue_.Push(stack.Pop());

	#ifdef DEBUG
		std::cout << queue_ << '\n';
	#endif

	return queue_;
}