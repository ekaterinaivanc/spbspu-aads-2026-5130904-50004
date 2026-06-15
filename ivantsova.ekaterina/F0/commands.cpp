#include <fstream>
#include <iostream>
#include "commands.hpp"
#include "utils.hpp"

ivantsova::Command::Command():
 handler(nullptr),
 minArgs(0)
{}

ivantsova::Command::Command(const std::string& n, ivantsova::CommandHandler h, size_t min, const std::string& syn):
 name(n),
 handler(h),
 minArgs(min),
 syntax(syn)
{}

bool ivantsova::Command::operator==(const Command& other) const
{
  return name == other.name;
}

void ivantsova::CommandRegistry::registerCommand(const std::string& name, ivantsova::CommandHandler handler,
  int minArgs, const std::string& syntax)
{
  Command cmd(name, handler, minArgs, syntax);
  commands.insert(name, cmd);
}

ivantsova::Command* ivantsova::CommandRegistry::findCommand(const std::string& name)
{
  return commands.find(name);
}

void ivantsova::CommandRegistry::execute(const char* input, ivantsova::GlobalUserRegistry& globalRegistry,
  ivantsova::NetworkManager& networkManager)
{
  std::string line(input);
  size_t start = 0;
  while (start < line.size() && (line[start] == ' ' || line[start] == '\t' || line[start] == '\n' || line[start] == '\r')) {
    ++start;
  }
  if (start == line.size()) {
    return;
  }
  size_t end = line.size() - 1;
  while (end > start && (line[end] == ' ' || line[end] == '\t' || line[end] == '\n' || line[end] == '\r')) {
    --end;
  }
  std::string trimmed = "";
  for (size_t i = start; i <= end; ++i) {
    trimmed += line[i];
  }
  ivantsova::MyVector< std::string > args;
  size_t pos = 0;
  while (pos < trimmed.size()) {
    while (pos < trimmed.size() && trimmed[pos] == ' ') {
      ++pos;
    }
    if (pos >= trimmed.size()) {
      break;
    }
    size_t endPos = pos;
    while (endPos < trimmed.size() && trimmed[endPos] != ' ') {
      ++endPos;
    }
    std::string arg = "";
    for (size_t i = pos; i < endPos; ++i) {
      arg += trimmed[i];
    }
    args.push_back(arg);
    pos = endPos;
  }
  if (args.size() == 0) {
    return;
  }
  Command* cmd = findCommand(args[0]);
  if (!cmd) {
    std::cout << "Unknown command. Type 'help'" << "\n";
    return;
  }
  if (args.size() < static_cast< size_t >(cmd->minArgs)) {
    std::cout << "ERROR: Not enough arguments. Usage: " << cmd->syntax << "\n";
    return;
  }
  cmd->handler(args, globalRegistry, networkManager);
}
