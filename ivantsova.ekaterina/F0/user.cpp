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

bool ivantsova::GlobalUserRegistry::registerUser(const User& user)
{
  if (users.find(user.id) != nullptr) {
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
  return users.find(id);
}

bool ivantsova::GlobalUserRegistry::userExists(const std::string& id) const
{
  return const_cast< GlobalUserRegistry* >(this)->users.find(id) != nullptr;
}
