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

void ivantsova::cmdShow(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry& globalRegistry,
  ivantsova::NetworkManager&)
{
  if (args.size() < 2) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string id(args[1]);

  ivantsova::User* u = globalRegistry.findUser(id);
  if (!u) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  std::cout << "ID: " << u->id << ", Last name: " << u->lastName << ", First name: " << u->firstName
    << ", City: " << u->city << ", Birthday: " << u->birthday << ", Friends: " << u->friends.size() << "\n";
}

void ivantsova::cmdUpdate(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry& globalRegistry, ivantsova::NetworkManager&)
{
  if (args.size() < 4) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string id(args[1]);
  const std::string field(args[2]);
  const std::string value(args[3]);

  ivantsova::User* u = globalRegistry.findUser(id);
  if (!u) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  if (field == "firstName") {
    u->firstName = value;
    std::cout << "First name updated!" << "\n";
  } else if (field == "lastName") {
    u->lastName = value;
    std::cout << "Last name updated!" << "\n";
  } else if (field == "city") {
    u->city = value;
    std::cout << "City updated!" << "\n";
  } else if (field == "birthday") {
    if (!ivantsova::isValidBirthday(value)) {
      std::cout << "ERROR: Invalid birthday format" << "\n";
      return;
    }
    u->birthday = value;
    std::cout << "Birthday updated!" << "\n";
  } else {
    std::cout << "ERROR: Invalid field. Allowed: firstName, lastName, city, birthday" << "\n";
  }
}

void ivantsova::cmdAddFriend(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 4) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id1(args[2]);
  const std::string id2(args[3]);

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  if (!net->hasUser(id1) || !net->hasUser(id2)) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  if (net->addFriend(id1, id2)) {
    std::cout << "Friends added!" << "\n";
  } else {
    std::cout << "ERROR: Already friends" << "\n";
  }
}

void ivantsova::cmdRemoveFriend(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 4) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id1(args[2]);
  const std::string id2(args[3]);

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  if (!net->hasUser(id1) || !net->hasUser(id2)) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  if (net->removeFriend(id1, id2)) {
    std::cout << "Friends removed!" << "\n";
  } else {
    std::cout << "ERROR: Not friends" << "\n";
  }
}

void ivantsova::cmdFriends(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 3) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id(args[2]);

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  ivantsova::User* u = net->getUser(id);
  if (!u) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  ivantsova::MyVector< ivantsova::User* > friends;
  net->getFriends(id, friends);

  std::cout << "Friends of " << id << " (" << u->lastName << " " << u->firstName << ") in " << netName << ":" << "\n";
  for (size_t i = 0; i < friends.size(); ++i) {
    std::cout << "  " << friends[i]->id << " | " << friends[i]->lastName << " " << friends[i]->firstName
      << " | " << friends[i]->city << "\n";
  }
  if (friends.size() == 0) {
    std::cout << "  No friends" << "\n";
  }
}

void ivantsova::cmdMutualFriends(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 4) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id1(args[2]);
  const std::string id2(args[3]);

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  if (!net->hasUser(id1) || !net->hasUser(id2)) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  ivantsova::MyVector< ivantsova::User* > mutual;
  net->getMutualFriends(id1, id2, mutual);

  std::cout << "Mutual friends of " << id1 << " and " << id2 << " in " << netName << ":" << "\n";
  for (size_t i = 0; i < mutual.size(); ++i) {
    std::cout << "  " << mutual[i]->id << " | " << mutual[i]->lastName << " " << mutual[i]->firstName
      << " | " << mutual[i]->city << "\n";
  }
  if (mutual.size() == 0) {
    std::cout << "  No mutual friends" << "\n";
  }
}

void ivantsova::cmdRecommend(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry& globalRegistry,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 3) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id(args[2]);

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  if (!net->hasUser(id)) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  ivantsova::MyVector< std::string > otherNets;
  networkManager.getAllNetworkNames(otherNets);

  std::cout << "Friend recommendations for " << id << " in " << netName << " (from other networks):" << "\n";

  int count = 0;
  for (size_t i = 0; i < otherNets.size() && count < 5; ++i) {
    if (otherNets[i] == netName) {
      continue;
    }

    ivantsova::SocialNetwork* otherNet = networkManager.getNetwork(otherNets[i]);
    if (!otherNet) {
      continue;
    }

    ivantsova::User* otherUser = otherNet->getUser(id);
    if (!otherUser) {
      continue;
    }

    for (size_t j = 0; j < otherUser->friends.size() && count < 5; ++j) {
      const std::string& friendId = otherUser->friends[j];
      if (!net->hasUser(friendId) && !net->areFriends(id, friendId)) {
        ivantsova::User* recommended = globalRegistry.findUser(friendId);
        if (recommended) {
          std::cout << "  " << ++count << ". " << friendId << " | " << recommended->lastName << " "
            << recommended->firstName << " - friends in " << otherNets[i] << "\n";
        }
      }
    }
  }

  if (count == 0) {
    std::cout << "  No recommendations found" << "\n";
  }
}

void ivantsova::cmdRecommendCity(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 3) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id(args[2]);

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  ivantsova::User* currentUser = net->getUser(id);
  if (!currentUser) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  ivantsova::MyVector< ivantsova::User* > allUsers;
  net->getAllUsers(allUsers);

  std::cout << "Same city recommendations for " << id << " (" << currentUser->lastName << " "
    << currentUser->firstName << ", " << currentUser->city << ") in " << netName << ":" << "\n";

  int count = 0;
  for (size_t i = 0; i < allUsers.size() && count < 5; ++i) {
    ivantsova::User* u = allUsers[i];
    if (u->id == id) {
      continue;
    }
    if (net->areFriends(id, u->id)) {
      continue;
    }
    if (u->city == currentUser->city) {
      std::cout << "  " << ++count << ". " << u->id << " | " << u->lastName << " " << u->firstName << "\n";
    }
  }

  if (count == 0) {
    std::cout << "  No recommendations found" << "\n";
  }
}

void ivantsova::cmdRecommendAge(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 4) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName(args[1]);
  const std::string id(args[2]);
  int ageDiff = std::atoi(args[3].c_str());

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  ivantsova::User* currentUser = net->getUser(id);
  if (!currentUser) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  int currentAge = ivantsova::calculateAge(currentUser->birthday);

  ivantsova::MyVector< ivantsova::User* > allUsers;
  net->getAllUsers(allUsers);

  std::cout << "Same age recommendations for " << id << " (" << currentUser->lastName << " "
    << currentUser->firstName << ", " << currentAge << " y.o.) in " << netName << ":" << "\n";

  int count = 0;
  for (size_t i = 0; i < allUsers.size() && count < 5; ++i) {
    ivantsova::User* u = allUsers[i];
    if (u->id == id) {
      continue;
    }
    if (net->areFriends(id, u->id)) {
      continue;
    }

    int otherAge = ivantsova::calculateAge(u->birthday);
    int diff = currentAge - otherAge;
    if (diff < 0) diff = -diff;

    if (diff <= ageDiff) {
      std::cout << "  " << ++count << ". " << u->id << " | " << u->lastName << " " << u->firstName
        << " (" << otherAge << " y.o.)" << "\n";
    }
  }

  if (count == 0) {
    std::cout << "  No recommendations found" << "\n";
  }
}

void ivantsova::cmdRecommendSimilar(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 4) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName = args[1];
  const std::string id = args[2];
  int ageDiff = std::atoi(args[3].c_str());

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  ivantsova::User* currentUser = net->getUser(id);
  if (!currentUser) {
    std::cout << "ERROR: User not found" << "\n";
    return;
  }

  int currentAge = ivantsova::calculateAge(currentUser->birthday);

  ivantsova::MyVector< ivantsova::User* > allUsers;
  net->getAllUsers(allUsers);

  std::cout << "Recommended friends for " << id << " (" << currentUser->lastName << " " << currentUser->firstName
    << ", " << currentUser->city << ", " << currentAge << " y.o.)" << " in " << netName << ":" << "\n";
  std::cout << "Matches (same city + age ± " << ageDiff << " years):" << "\n";

  int count = 0;
  for (size_t i = 0; i < allUsers.size() && count < 5; ++i) {
    ivantsova::User* u = allUsers[i];
    if (u->id == id) {
      continue;
    }
    if (net->areFriends(id, u->id)) {
      continue;
    }
    if (u->city != currentUser->city) {
      continue;
    }

    int otherAge = ivantsova::calculateAge(u->birthday);
    int diff = currentAge - otherAge;
    if (diff < 0) diff = -diff;

    if (diff <= ageDiff) {
      std::cout << "  " << ++count << ". " << u->id << " | " << u->lastName << " " << u->firstName
        << " - " << u->city << ", " << otherAge << " y.o.";
      if (diff == 0) {
        std::cout << " (same age)" << "\n";
      } else {
        std::cout << " (diff: " << diff << " year(s))" << "\n";
      }
    }
  }

  if (count == 0) {
    std::cout << "  No recommendations found" << "\n";
  }
}

void ivantsova::cmdExport(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry&,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 3) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string netName = args[1];
  const std::string filename = args[2];

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }

  if (net->exportToFile(filename)) {
    std::cout << "Exported to " << filename << " (" << net->getUserCount() << " users saved)" << "\n";
  } else {
    std::cout << "ERROR: Cannot create file" << "\n";
  }
}

void ivantsova::cmdImport(const MyVector< std::string >& args, ivantsova::GlobalUserRegistry& globalRegistry,
  ivantsova::NetworkManager& networkManager)
{
  if (args.size() < 3) {
    std::cout << "ERROR: invalid command" << "\n";
    return;
  }

  const std::string filename(args[1]);
  const std::string netName(args[2]);

  ivantsova::SocialNetwork* net = networkManager.getNetwork(netName);
  if (!net) {
    std::cout << "ERROR: Network not found" << "\n";
    return;
  }
  int duplicates = 0;
  int result = net->importFromFile(filename, globalRegistry, duplicates);
  if (result == -1) {
    std::cout << "ERROR: File not found" << "\n";
  } else {
    std::cout << "Imported from " << filename << " (" << result << " users added, "
      << duplicates << " duplicate skipped)" << "\n";
  }
}
