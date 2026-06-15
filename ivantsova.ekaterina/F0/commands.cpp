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

void ivantsova::CommandRegistry::printHelp()
{
  ivantsova::MyVector< std::string > keys;
  commands.getAllKeys(keys);

  std::cout << "\n=== Available commands ===" << "\n";
  for (size_t i = 0; i < keys.size(); ++i) {
    Command* cmd = commands.find(keys[i]);
    if (cmd) {
      std::cout << "  " << cmd->syntax << "\n";
    }
  }
  std::cout << "\n";
}

void ivantsova::cmdAdd(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry& globalRegistry,
  ivantsova::NetworkManager&)
{
  if (args.size() < 6) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string id(args[1]);
  const std::string ln(args[2]);
  const std::string fn(args[3]);
  const std::string city(args[4]);
  const std::string bd(args[5]);

  if (!ivantsova::isValidBirthday(bd)) {
    std::cout << "ERROR: Invalid birthday format" << "\n";
    return;
  }

  ivantsova::User newUser(id, ln, fn, city, bd);
  if (globalRegistry.registerUser(newUser)) {
    std::cout << "User added!" << "\n";
  } else {
    std::cout << "ERROR: This ID is busy" << "\n";
  }
}

void ivantsova::cmdAddNet(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&, ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 2) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);

  if (networkManager.addNetwork(netName)) {
    std::cout << "Network '" << netName << "' created" << "\n";
  } else {
    std::cout << "ERROR: Social network '" << netName << "' already exists" << "\n";
  }
}

void ivantsova::cmdRegister(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry& globalRegistry,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 3) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id(args[2]);

  ivantsova::User* user = globalRegistry.findUser(id);
  if (!user) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  if (net->addUser(user)) {
    std::cout << "User registered!" << "\n";
  } else {
    std::cout << "ERROR: User already in this network" << "\n";
  }
}

void ivantsova::cmdDelete(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry& globalRegistry,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 2) {
    std::cout << "ERROR: invalid command>" << "\n";
    return;
  }

  const std::string id(args[1]);

  ivantsova::User* user = globalRegistry.findUser(id);
  if (!user) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  ivantsova::MyVector< std::string > netNames;
  networkManager.getAllNetworkNames(netNames);

  for (size_t i = 0; i < netNames.size(); ++i) {
    ivantsova::SocialNetwork* net = networkManager.getNetwork(netNames[i]);
    if (net && net->hasUser(id)) {
      net->removeUser(id);
    }
  }

  globalRegistry.unregisterUser(id);
  std::cout << "User deleted" << "\n";
}
