#include <iostream>
#include "user.hpp"

ivantsova::User::User() {}

ivantsova::User::User(const std::string& uid, const std::string& ln,
  const std::string& fn, const std::string& c, const std::string& bd):
   id(uid),
   lastName(ln),
   firstName(fn),
   city(c),
   birthday(bd)
{}

bool ivantsova::User::operator==(const User& other) const
{
  return id == other.id;
}

std::ostream& ivantsova::operator<<(std::ostream& os, const ivantsova::User& user)
{
  os << user.id << " | " << user.lastName << " " << user.firstName
    << " | " << user.city << " | " << user.birthday;
  return os;
}

bool ivantsova::GlobalUserRegistry::registerUser(const User& user)
{
  if (users.findExists(user.id)) {
    return false;
  }
  return users.insert(user.id, user);
}

bool ivantsova::GlobalUserRegistry::unregisterUser(const std::string& id)
{
  return users.remove(id);
}

ivantsova::User* ivantsova::GlobalUserRegistry::findUser(const std::string& id)
{
  auto it = users.find(id);
  return (it != users.end()) ? &it->value : nullptr;
}

bool ivantsova::GlobalUserRegistry::userExists(const std::string& id) const
{
  return users.findExists(id);
}
