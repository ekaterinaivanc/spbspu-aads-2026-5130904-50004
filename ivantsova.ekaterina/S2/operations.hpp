#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace ivantsova {
  bool higherPriority(const std::string& op1, const std::string& op2);
  bool isOperator(const std::string& token);
  long long useOperation(long long a, long long b, const std::string& line);
  Queue< std::string > convertToPostfix(const std::string& line);
  long long calculatePostfix(Queue< std::string >& postfix);
  long long calculateExpression(const std::string& line);
};

#endif
