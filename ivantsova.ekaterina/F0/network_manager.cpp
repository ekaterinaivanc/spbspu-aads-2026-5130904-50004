#include "network_manager.hpp"

bool ivantsova::NetworkManager::addNetwork(const std::string& name)
{
  if (networks.findExists(name)) {
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
  auto it = networks.find(name);
  return (it != networks.end()) ? &it->value : nullptr;
}

bool ivantsova::NetworkManager::networkExists(const std::string& name) const
{
  return networks.find(name) != networks.end();
}

void ivantsova::NetworkManager::getAllNetworkNames(ivantsova::MyVector< std::string >& names)
{
  networks.getAllKeys(names);
}
