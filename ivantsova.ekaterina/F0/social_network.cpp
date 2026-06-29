#include <fstream>
#include <cstring>
#include "social_network.hpp"
#include "utils.hpp"

ivantsova::SocialNetwork::SocialNetwork()
{}

ivantsova::SocialNetwork::SocialNetwork(const std::string& n):
 name(n)
{}

const std::string& ivantsova::SocialNetwork::getName() const
{
  return name;
}

bool ivantsova::SocialNetwork::addUser(User* user)
{
  if (!user) {
    return false;
  }
  if (networkUsers.findExists(user->id)) {
    return false;
  }
  return networkUsers.insert(user->id, user);
}

bool ivantsova::SocialNetwork::removeUser(const std::string& id)
{
  return networkUsers.remove(id);
}

bool ivantsova::SocialNetwork::hasUser(const std::string& id) const
{
  return networkUsers.findExists(id);
}

ivantsova::User* ivantsova::SocialNetwork::getUser(const std::string& id)
{
  auto it = networkUsers.find(id);
  return (it != networkUsers.end()) ? it->value : nullptr;
}

bool ivantsova::SocialNetwork::addFriend(const std::string& id1, const std::string& id2)
{
  auto it1 = networkUsers.find(id1);
  auto it2 = networkUsers.find(id2);
  if (it1 == networkUsers.end() || it2 == networkUsers.end()) {
    return false;
  }
  User* u1 = it1->value;
  User* u2 = it2->value;
  for (size_t i = 0; i < u1->friends.size(); ++i) {
    if (u1->friends[i] == id2) {
      return false;
    }
  }
  u1->friends.push_back(id2);
  u2->friends.push_back(id1);
  return true;
}

bool ivantsova::SocialNetwork::removeFriend(const std::string& id1, const std::string& id2)
{
  auto it1 = networkUsers.find(id1);
  auto it2 = networkUsers.find(id2);
  if (it1 == networkUsers.end() || it2 == networkUsers.end()) {
    return false;
  }
  User* u1 = it1->value;
  User* u2 = it2->value;
  u1->friends.remove(id2);
  u2->friends.remove(id1);
  return true;
}

bool ivantsova::SocialNetwork::areFriends(const std::string& id1, const std::string& id2) const
{
  auto it1 = networkUsers.find(id1);
  if (it1 == networkUsers.end()) {
    return false;
  }
  User* u1 = it1->value;
  for (size_t i = 0; i < u1->friends.size(); ++i) {
    if (u1->friends[i] == id2) {
      return true;
    }
  }
  return false;
}

void ivantsova::SocialNetwork::getFriends(const std::string& id, ivantsova::MyVector< User* >& result)
{
  result.clear();
  auto it = networkUsers.find(id);
  if (it == networkUsers.end()) {
    return;
  }
  User* u = it->value;
  for (size_t i = 0; i < u->friends.size(); ++i) {
    auto fit = networkUsers.find(u->friends[i]);
    if (fit != networkUsers.end()) {
      result.push_back(fit->value);
    }
  }
}

void ivantsova::SocialNetwork::getMutualFriends(const std::string& id1, const std::string& id2, ivantsova::MyVector< User* >& result)
{
  result.clear();
  auto it1 = networkUsers.find(id1);
  auto it2 = networkUsers.find(id2);
  if (it1 == networkUsers.end() || it2 == networkUsers.end()) {
    return;
  }
  User* u1 = it1->value;
  User* u2 = it2->value;
  for (size_t i = 0; i < u1->friends.size(); ++i) {
    for (size_t j = 0; j < u2->friends.size(); ++j) {
      if (u1->friends[i] == u2->friends[j]) {
        auto mit = networkUsers.find(u1->friends[i]);
        if (mit != networkUsers.end()) {
          result.push_back(mit->value);
        }
      }
    }
  }
}

void ivantsova::SocialNetwork::getAllUsers(ivantsova::MyVector< User* >& result)
{
  result.clear();
  ivantsova::MyVector< std::string > keys;
  networkUsers.getAllKeys(keys);
  for (size_t i = 0; i < keys.size(); ++i) {
    auto it = networkUsers.find(keys[i]);
    if (it != networkUsers.end()) {
      result.push_back(it->value);
    }
  }
}

size_t ivantsova::SocialNetwork::getUserCount() const
{
  return networkUsers.getCount();
}

void ivantsova::SocialNetwork::clear()
{
  networkUsers.clear();
}

bool ivantsova::SocialNetwork::exportToFile(const std::string& filename)
{
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  ivantsova::MyVector< User* > allUsers;
  getAllUsers(allUsers);

  for (size_t i = 0; i < allUsers.size(); ++i) {
    User* u = allUsers[i];
    file << u->id << "|" << u->lastName << "|" << u->firstName << "|"
         << u->city << "|" << u->birthday << "|";
    for (size_t j = 0; j < u->friends.size(); ++j) {
      if (j > 0) {
        file << ",";
      }
      file << u->friends[j];
    }
    file << "\n";
  }
  file.close();
  return true;
}

int ivantsova::SocialNetwork::importFromFile(const std::string& filename, GlobalUserRegistry& globalRegistry,  int& duplicates)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    return -1;
  }
  int added = 0;
  duplicates = 0;
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    size_t pos = 0;
    size_t delim = line.find('|');
    if (delim == std::string::npos) {
      continue;
    }
    std::string id = "";
    for (size_t i = pos; i < delim; ++i) {
      id += line[i];
    }

    pos = delim + 1;
    delim = line.find('|', pos);
    if (delim == std::string::npos) {
      continue;
    }
    std::string ln = "";
    for (size_t i = pos; i < delim; ++i) {
      ln += line[i];
    }

    pos = delim + 1;
    delim = line.find('|', pos);
    if (delim == std::string::npos) {
      continue;
    }
    std::string fn = "";
    for (size_t i = pos; i < delim; ++i) {
      fn += line[i];
    }
    pos = delim + 1;
    delim = line.find('|', pos);
    if (delim == std::string::npos) {
      continue;
    }
    std::string city = "";
    for (size_t i = pos; i < delim; ++i) {
      city += line[i];
    }

    pos = delim + 1;
    delim = line.find('|', pos);
    std::string bd = "";
    if (delim == std::string::npos) {
      for (size_t i = pos; i < line.size(); ++i) {
        bd += line[i];
      }
    } else {
      for (size_t i = pos; i < delim; ++i) {
        bd += line[i];
      }
    }

    if (!isValidBirthday(bd)) {
      continue;
    }

    User* existing = globalRegistry.findUser(id);
    if (!existing) {
      User newUser(id, ln, fn, city, bd);
      globalRegistry.registerUser(newUser);
      existing = globalRegistry.findUser(id);
    }

    if (networkUsers.findExists(id)) {
      ++duplicates;
      continue;
    }

    if (existing && networkUsers.insert(id, existing)) {
      ++added;
    }
  }
  file.close();
  return added;
}
