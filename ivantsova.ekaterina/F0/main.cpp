#include <iostream>
#include "commands.hpp"

void registerAllCommands(ivantsova::CommandRegistry& registry)
{
  registry.registerCommand("add", ivantsova::cmdAdd, 6,
    "add <id> <lastName> <firstName> <city> <birthday>");
  registry.registerCommand("addNet", ivantsova::cmdAddNet, 2,
    "addNet <network>");
  registry.registerCommand("register", ivantsova::cmdRegister, 3,
    "register <network> <id>");
  registry.registerCommand("delete", ivantsova::cmdDelete, 2,
    "delete <id>");
  registry.registerCommand("show", ivantsova::cmdShow, 2,
    "show <id>");
  registry.registerCommand("update", ivantsova::cmdUpdate, 4,
    "update <id> <field> <value>");
  registry.registerCommand("add-friend", ivantsova::cmdAddFriend, 4,
    "add-friend <network> <id1> <id2>");
  registry.registerCommand("remove-friend", ivantsova::cmdRemoveFriend, 4,
    "remove-friend <network> <id1> <id2>");
  registry.registerCommand("friends", ivantsova::cmdFriends, 3,
    "friends <network> <id>");
  registry.registerCommand("mutual-friends", ivantsova::cmdMutualFriends, 4,
    "mutual-friends <network> <id1> <id2>");
  registry.registerCommand("recommend", ivantsova::cmdRecommend, 3,
    "recommend <network> <id>");
  registry.registerCommand("recommend-city", ivantsova::cmdRecommendCity, 3,
    "recommend-city <network> <id>");
  registry.registerCommand("recommend-age", ivantsova::cmdRecommendAge, 3,
    "recommend-age <network> <id> <ageDiff>");
  registry.registerCommand("recommend-similar", ivantsova::cmdRecommendSimilar, 3,
    "recommend-similar <network> <id> <ageDiff>");
  registry.registerCommand("export", ivantsova::cmdExport, 3,
    "export <network> <filename>");
  registry.registerCommand("import", ivantsova::cmdImport, 3,
    "import <filename> <network>");
  registry.registerCommand("clear", ivantsova::cmdClear, 2,
    "clear <network>");
  registry.registerCommand("deleteNet", ivantsova::cmdDeleteNet, 2,
    "deleteNet <network>");
  registry.registerCommand("help", ivantsova::cmdHelp, 1, "help");
  registry.registerCommand("exit", ivantsova::cmdExit, 1, "exit");
}

int main()
{
  ivantsova::CommandRegistry registry;
  registerAllCommands(registry);

  ivantsova::GlobalUserRegistry globalRegistry;
  ivantsova::NetworkManager networkManager;

  std::cout << "=== Social Network Manager ===" << "\n";
  std::cout << "Type 'help' for available commands" << "\n";
  std::cout << "First, create a network: addNet <name>" << "\n";
  std::cout << "\n";

  char input[ivantsova::MAX_COMMAND_LEN];

  while (true) {
    std::cout << "> ";
    std::cin.getline(input, ivantsova::MAX_COMMAND_LEN);
    if (!std::cin) {
      break;
    }
    if (input[0] == '\0') {
      continue;
    }
    registry.execute(input, globalRegistry, networkManager);
  }
  return 0;
}
