#ifndef SOCIAL_NETWORK_HPP
#define SOCIAL_NETWORK_HPP

#include <string>
#include "vector.hpp"
#include "hash_table.hpp"
#include "user.hpp"

namespace ivantsova {

  class SocialNetwork
  {
  private:
    std::string name;
    DoubleHashTable< std::string, User* > networkUsers;

  public:
    SocialNetwork();
    SocialNetwork(const std::string& name);

    const std::string& getName() const;
    bool addUser(User* user);
    bool removeUser(const std::string& id);
    bool hasUser(const std::string& id) const;
    User* getUser(const std::string& id);

    bool addFriend(const std::string& id1, const std::string& id2);
    bool removeFriend(const std::string& id1, const std::string& id2);
    bool areFriends(const std::string& id1, const std::string& id2) const;
    void getFriends(const std::string& id, MyVector< User* >& result);
    void getMutualFriends(const std::string& id1, const std::string& id2, MyVector< User* >& result);

    void getAllUsers(MyVector< User* >& result);
    size_t getUserCount() const;
    void clear();

    bool exportToFile(const std::string& filename);
    int importFromFile(const std::string& filename, GlobalUserRegistry& globalRegistry,  int& duplicates);
  };
}

#endif
