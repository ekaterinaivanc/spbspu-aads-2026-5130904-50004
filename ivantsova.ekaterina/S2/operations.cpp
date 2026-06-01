#include <sstream>
#include <cctype>
#include <limits>
#include "operations.hpp"

bool ivantsova::higherPriority(const std::string& op1, const std::string& op2)
{
  int p1 = 0;
  if (op1 == "+" || op1 == "-") {
    p1 = 1;
  } else if (op1 == "*" || op1 == "/" || op1 == "%") {
    p1 = 2;
  } else if (op1 == "<<") {
    p1 = 3;
  }
  int p2 = 0;
  if (op2 == "+" || op2 == "-") {
    p2 = 1;
  } else if (op2 == "*" || op2 == "/" || op2 == "%") {
    p2 = 2;
  } else if (op2 == "<<") {
    p2 = 3;
  }
  return p1 >= p2;
}

bool ivantsova::isOperator(const std::string& token)
{
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "<<";
}

long long ivantsova::useOperation(long long a, long long b, const std::string& op)
{
  if (op == "+") {
    if (b > 0 && a > std::numeric_limits< long long >::max() - b) {
      throw std::overflow_error("Addition overflow");
    }
    if (b < 0 && a < std::numeric_limits< long long >::min() - b) {
      throw std::underflow_error("Addition underflow");
    }
    return a + b;
  }
  if (op == "-") {
    if (b < 0 && a > std::numeric_limits< long long >::max() + b) {
      throw std::overflow_error("Substraction overflow");
    }
    if (b > a && a < std::numeric_limits< long long >::min() + b) {
      throw std::underflow_error("Substraction underflow");
    }
    return a - b;
  }
  if (op == "*") {
    if (a != 0 && b != 0) {
      const long long maxx = std::numeric_limits< long long >::max();
      const long long minn = std::numeric_limits< long long >::min();
      if ((a > 0 && b > 0 && a > maxx / b) || (a < 0 && b < 0 && a < maxx / b)) {
        throw std::overflow_error("Multiplication overflow");
      }
      if ((a > 0 && b < 0 && b < minn / a) || (a < 0 && b > 0 && a < minn / b)) {
        throw std::underflow_error("Multiplication underflow");
      }
    }
    return a * b;
  }
  if (op == "/" || op == "%") {
    if (b == 0) {
      throw std::runtime_error("Division by zero");
    }
    if (op == "/") {
      return a / b;
    }
    if (a % b < 0) {
      if (b > 0) {
        return a % b + b;
      } else {
        return a % b - b;
      }
    }
    return a % b;
  }
  if (op == "<<") {
    if (b < 0) {
      throw std::runtime_error("Shift by negative number");
    }
    if (b >= static_cast< int >(sizeof(long long) * 8)) {
      throw std::overflow_error("Shift number too large");
    }
    return a << b;
  }
  throw std::runtime_error("Unknown operator");
}

ivantsova::Queue< std::string > ivantsova::convertToPostfix(const std::string& line)
{
  std::stringstream ss(line);
  std::string token;
  Queue< std::string > input;
  while (ss >> token) {
    input.push(token);
  }
  if (input.empty()) {
    throw std::runtime_error("Empty expression");
  }
  Queue< std::string > output;
  Stack< std::string > operators;
  while (!input.empty()) {
    std::string tok = input.pop();
    try {
      std::stoll(tok);
      output.push(tok);
    } catch (...) {
      if (tok == "(") {
        operators.push(tok);
      } else if (tok == ")") {
        while (!operators.empty() && operators.top() != "(") {
          output.push(operators.pop());
        }
        if (operators.empty()) {
          throw std::runtime_error("Mismatched parentheses");
        }
        operators.pop();
      } else if (isOperator(tok)) {
        while (!operators.empty() && operators.top() != "(" && higherPriority(operators.top(), tok)) {
          output.push(operators.pop());
        }
        operators.push(tok);
      } else {
        throw std::runtime_error("Invalid token");
      }
    }
  }
  while (!operators.empty()) {
    if (operators.top() == "(" || operators.top() == ")") {
      throw std::runtime_error("Mismatched parentheses");
    }
    output.push(operators.pop());
  }
  return output;
}

long long ivantsova::calculatePostfix(Queue< std::string >& postfix)
{
  Stack< long long > evalStack;
  while (!postfix.empty()) {
    std::string tok = postfix.pop();
    try {
      evalStack.push(std::stoll(tok));
    } catch (...) {
      if (isOperator(tok)) {
        if (evalStack.size() < 2) {
          throw std::runtime_error("Not enough operands");
        }
        long long b = evalStack.pop();
        long long a = evalStack.pop();
        evalStack.push(useOperation(a, b, tok));
      } else {
        throw std::runtime_error("Invalid token in postfix");
      }
    }
  }
  if (evalStack.size() != 1) {
    throw std::runtime_error("Invalid expression");
  }
  return evalStack.pop();
}

long long ivantsova::calculateExpression(const std::string& line)
{
  Queue< std::string > postfix = convertToPostfix(line);
  return calculatePostfix(postfix);
}
