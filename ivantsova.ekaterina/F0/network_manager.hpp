#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <string>
#include "hash_table.hpp"
#include "social_network.hpp"

namespace ivantsova {

  class NetworkManager
  {
  private:
    DoubleHashTable< std::string, SocialNetwork > networks;

  public:
    bool addNetwork(const std::string& name);
    bool removeNetwork(const std::string& name);
    SocialNetwork* getNetwork(const std::string& name);
    bool networkExists(const std::string& name) const;
    void getAllNetworkNames(MyVector< std::string >& names);
  };
}

#endif
