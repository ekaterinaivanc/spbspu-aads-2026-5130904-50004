#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include "vector.hpp"
#include "hash_table.hpp"

namespace ivantsova {

  struct User
  {
    std::string id;
    std::string lastName;
    std::string firstName;
    std::string city;
    std::string birthday;
    MyVector< std::string > friends;

    User();
    User(const std::string& uid, const std::string& ln,
      const std::string& fn, const std::string& c, const std::string& bd);
    bool operator==(const User& other) const;
    friend std::ostream& operator<<(std::ostream& os, const User& user);
  };

  std::ostream& operator<<(std::ostream& os, const User& user);

  class GlobalUserRegistry
  {
  private:
    DoubleHashTable< std::string, User > users;
  public:
    bool registerUser(const User& user);
    bool unregisterUser(const std::string& id);
    User* findUser(const std::string& id);
    bool userExists(const std::string& id) const;
  };
}

#endif
