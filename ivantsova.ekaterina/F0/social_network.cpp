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
  if (networkUsers.find(user->id) != nullptr) {
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
  return networkUsers.find(id) != nullptr;
}

ivantsova::User* ivantsova::SocialNetwork::getUser(const std::string& id)
{
  User** res = networkUsers.find(id);
  return res ? *res : nullptr;
}

bool ivantsova::SocialNetwork::addFriend(const std::string& id1, const std::string& id2)
{
  User** u1Ptr = networkUsers.find(id1);
  User** u2Ptr = networkUsers.find(id2);
  if (!u1Ptr || !u2Ptr) {
    return false;
  }
  User* u1 = *u1Ptr;
  User* u2 = *u2Ptr;
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
  User** u1Ptr = networkUsers.find(id1);
  User** u2Ptr = networkUsers.find(id2);
  if (!u1Ptr || !u2Ptr) {
    return false;
  }
  User* u1 = *u1Ptr;
  User* u2 = *u2Ptr;
  u1->friends.remove(id2);
  u2->friends.remove(id1);
  return true;
}

bool ivantsova::SocialNetwork::areFriends(const std::string& id1, const std::string& id2) const
{
  auto u1Ptr = networkUsers.find(id1);
  if (!u1Ptr) {
    return false;
  }
  User* u1 = *u1Ptr;
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
  User** uPtr = networkUsers.find(id);
  if (!uPtr) {
    return;
  }
  User* u = *uPtr;
  for (size_t i = 0; i < u->friends.size(); ++i) {
    User** friendPtr = networkUsers.find(u->friends[i]);
    if (friendPtr) {
      result.push_back(*friendPtr);
    }
  }
}

void ivantsova::SocialNetwork::getMutualFriends(const std::string& id1, const std::string& id2, ivantsova::MyVector< User* >& result)
{
  result.clear();
  User** u1Ptr = networkUsers.find(id1);
  User** u2Ptr = networkUsers.find(id2);
  if (!u1Ptr || !u2Ptr) {
    return;
  }
  User* u1 = *u1Ptr;
  User* u2 = *u2Ptr;
  for (size_t i = 0; i < u1->friends.size(); ++i) {
    for (size_t j = 0; j < u2->friends.size(); ++j) {
      if (u1->friends[i] == u2->friends[j]) {
        User** mutualPtr = networkUsers.find(u1->friends[i]);
        if (mutualPtr) {
          result.push_back(*mutualPtr);
        }
        break;
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
    User** uPtr = networkUsers.find(keys[i]);
    if (uPtr) {
      result.push_back(*uPtr);
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

    if (networkUsers.find(id) != nullptr) {
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
