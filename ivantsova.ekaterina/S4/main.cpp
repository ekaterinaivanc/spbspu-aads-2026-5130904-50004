#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "commands.hpp"

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "Incorrect number of arguments" << "\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open()) {
    std::cerr << "Cannot open file" << "\n";
    return 1;
  }
  ivantsova::Datasets datasets;
  std::string token;
  std::string currentName;

  while (in >> token) {
    bool isNum = true;
    for (char c : token) {
      if (c == '-' || c == '+') {
        continue;
      }
      if (!isdigit(c)) {
        isNum = false;
        break;
      }
    }

    if (isNum) {
      int key = std::stoi(token);
      std::string value;
      if (in >> value) {
        auto it = datasets.find(currentName);
        if (it != datasets.end()) {
          it->second.push(key, value);
        }
      }
    } else {
      currentName = token;
      if (datasets.find(currentName) == datasets.end()) {
        datasets.push(currentName, ivantsova::Dictionary());
      }
    }
  }
  in.close();

  ivantsova::BSTree< std::string, ivantsova::Command > cmds;
  cmds.push("print", ivantsova::cmdPrint);
  cmds.push("complement", ivantsova::cmdComplement);
  cmds.push("intersect", ivantsova::cmdIntersect);
  cmds.push("union", ivantsova::cmdUnion);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    auto it = cmds.find(cmd);
    if (it == cmds.end()) {
      std::cout << "<INVALID COMMAND>" << "\n";
      continue;
    }
    try {
      it->second(iss, std::cout, datasets);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << "\n";
    }
  }
  return 0;
}
