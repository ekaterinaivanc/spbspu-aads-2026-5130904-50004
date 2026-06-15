#include "network_manager.hpp"

bool ivantsova::NetworkManager::addNetwork(const std::string& name)
{
  if (networks.find(name) != nullptr) {
    return false;
  }
  SocialNetwork net(name);
  return networks.insert(name, net);
}

bool ivantsova::NetworkManager::removeNetwork(const std::string& name)
{
  return networks.remove(name);
}

ivantsova::SocialNetwork* ivantsova::NetworkManager::getNetwork(const std::string& name)
{
  return networks.find(name);
}

bool ivantsova::NetworkManager::networkExists(const std::string& name) const
{
  return const_cast< NetworkManager* >(this)->networks.find(name) != nullptr;
}

void ivantsova::NetworkManager::getAllNetworkNames(ivantsova::MyVector< std::string >& names)
{
  networks.getAllKeys(names);
}
