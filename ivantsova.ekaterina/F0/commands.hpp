#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <stdexcept>
#include "user.hpp"
#include "network_manager.hpp"

namespace ivantsova {

  const int MAX_COMMAND_LEN = 512;

  typedef void (*CommandHandler)(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry,
  NetworkManager& networkManager);

  struct Command
  {
    std::string name;
    CommandHandler handler;
    size_t minArgs;
    std::string syntax;

    Command();
    Command(const std::string& name, CommandHandler handler, size_t minArgs, const std::string& syntax);
    bool operator==(const Command& other) const;
  };

  class CommandRegistry
  {
  private:
    DoubleHashTable< std::string, Command > commands;

  public:
    void registerCommand(const std::string& name, CommandHandler handler, size_t minArgs, const std::string& syntax);
    DoubleHashTable< std::string, Command >::iterator findCommand(const std::string& name);
    void execute(const char* input, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
    void printHelp();
};

  void cmdAdd(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdAddNet(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdRegister(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdDelete(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdShow(const MyVector< std::string >& args,GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdUpdate(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdAddFriend(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdRemoveFriend(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdFriends(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdMutualFriends(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdRecommend(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdRecommendCity(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdRecommendAge(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdRecommendSimilar(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdExport(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdImport(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdClear(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdDeleteNet(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdExit(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);
  void cmdHelp(const MyVector< std::string >& args, GlobalUserRegistry& globalRegistry, NetworkManager& networkManager);

}

#endif
