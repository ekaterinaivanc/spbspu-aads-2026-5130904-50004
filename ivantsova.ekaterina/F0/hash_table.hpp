#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <string>
#include "vector.hpp"

namespace ivantsova {

  const double LOAD_FACTOR_LIMIT = 0.7;
  const int HASH_MULTIPLIER = 31;
  const int DEFAULT_TABLE_SIZE = 101;

  enum State { EMPTY, OCCUPIED, DELETED };

  struct StringHash
  {
    size_t operator()(const std::string& key) const
    {
      size_t h = 0;
      for (char c : key) {
        h = (h * HASH_MULTIPLIER + static_cast< size_t >(c));
      }
      return h;
    }
  };

  struct StringEqual
  {
    bool operator()(const std::string& a, const std::string& b) const
    {
      return a == b;
    }
  };

  template< class Key, class Value, class Hash = StringHash, class Equal = StringEqual >
  class DoubleHashTable
  {
  private:
    struct Entry
    {
      Key key;
      Value value;
      State state;
      Entry():
       state(EMPTY)
      {}
    };

    Entry* table;
    size_t size;
    size_t count;
    Hash hasher;
    Equal equal;

    size_t hash1(const Key& key) const
    {
      return hasher(key) % (size);
    }

    size_t hash2(const Key& key) const
    {
      return 1 + (hasher(key) % (size - 1));
    }

    size_t findIndex(const Key& key) const
    {
      size_t idx = hash1(key);
      size_t step = hash2(key);
      size_t start = idx;
      size_t i = 0;

      while (table[idx].state != EMPTY && i < size) {
        if (table[idx].state == OCCUPIED && equal(table[idx].key, key)) {
          return idx;
        }
        ++i;
        idx = (start + i * step) % size;
      }
      return static_cast< size_t >(-1);
    }

    bool isPrime(size_t n) const
    {
      if (n < 2) {
        return false;
      }
      for (size_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
          return false;
        }
      }
      return true;
    }

    size_t nextPrime(size_t n) const
    {
      while (!isPrime(n)) {
        ++n;
      }
      return n;
    }

    void rehash()
    {
      size_t newSize = nextPrime(size * 2);
      Entry* oldTable = table;
      size_t oldSize = size;

      table = new Entry[newSize];
      size = newSize;
      count = 0;

      for (size_t i = 0; i < oldSize; ++i) {
        if (oldTable[i].state == OCCUPIED) {
          insert(oldTable[i].key, oldTable[i].value);
        }
      }
      delete[] oldTable;
    }

  public:
    DoubleHashTable(size_t initialSize = DEFAULT_TABLE_SIZE):
     size(nextPrime(initialSize)),
     count(0)
    {
      table = new Entry[size];
    }

    DoubleHashTable(const DoubleHashTable& other):
     size(other.size),
     count(other.count),
     hasher(other.hasher),
     equal(other.equal)
    {
      table = new Entry[size];
      for (size_t i = 0; i < size; ++i) {
        table[i] = other.table[i];
      }
    }

    DoubleHashTable& operator=(const DoubleHashTable& other)
    {
      if (this != &other) {
        delete[] table;
        size = other.size;
        count = other.count;
        hasher = other.hasher;
        equal = other.equal;
        table = new Entry[size];
        for (size_t i = 0; i < size; ++i) {
          table[i] = other.table[i];
        }
      }
      return *this;
    }

    ~DoubleHashTable()
    {
      delete[] table;
    }

    bool insert(const Key& key, const Value& value)
    {
      if (findIndex(key) != static_cast< size_t >(-1)) {
        return false;
      }
      if (static_cast< double >(count) / size > LOAD_FACTOR_LIMIT) {
        rehash();
      }

      size_t idx = hash1(key);
      size_t step = hash2(key);
      size_t start = idx;
      size_t i = 0;
      while (table[idx].state == OCCUPIED && i < size) {
        ++i;
        idx = (start + i * step) % size;
      }
      table[idx].key = key;
      table[idx].value = value;
      table[idx].state = OCCUPIED;
      ++count;
      return true;
    }

    Value* find(const Key& key)
    {
      size_t idx = findIndex(key);
      return (idx != static_cast< size_t >(-1)) ? &table[idx].value : nullptr;
    }

    const Value* find(const Key& key) const
    {
      size_t idx = findIndex(key);
      return (idx != static_cast< size_t >(-1)) ? &table[idx].value : nullptr;
    }

    bool remove(const Key& key)
    {
      size_t idx = findIndex(key);
      if (idx == static_cast< size_t >(-1)) {
        return false;
      }
      table[idx].state = DELETED;
      --count;
      return true;
    }

    void getAllKeys(MyVector< Key >& keys) const
    {
      keys.clear();
      for (size_t i = 0; i < size; ++i) {
        if (table[i].state == OCCUPIED) {
          keys.push_back(table[i].key);
        }
      }
    }

    size_t getCount() const
    {
      return count;
    }

    void clear()
    {
      delete[] table;
      table = new Entry[size];
      count = 0;
    }
  };
}

#endif
