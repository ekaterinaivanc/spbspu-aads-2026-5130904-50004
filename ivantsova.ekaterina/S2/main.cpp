#include <iostream>
#include <fstream>
#include "operations.hpp"

int main(int argc, char* argv[])
{
  try {
    std::ifstream file;
    std::istream& input = (argc >= 2) ? file : std::cin;
    if (argc >= 2) {
      file.open(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Cannot open file" << "\n";
        return 1;
      }
    }
    long long results[100];
    size_t count = 0;
    std::string line;
    while (std::getline(input, line)) {
      if (line.empty() || line.find_first_not_of(" \t") == std::string::npos) {
        continue;
      }
      results[count++] = ivantsova::calculateExpression(line);
    }
    if (count > 0) {
      std::cout << results[count - 1];
      for (size_t i = 1; i < count; ++i) {
        std::cout << " " << results[count - 1 - i];
      }
      std::cout << "\n";
    } else {
        std::cout << "\n";
    }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
