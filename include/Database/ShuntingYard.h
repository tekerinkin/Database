#pragma once
#include "Map/Map.h"
#include "Queue.h"
#include "Stack.h"
#include "util/String_util.h"
#include <string>
#include <vector>

queue::Queue<std::string> InfixToPostfix(const std::vector<std::string>& infix);