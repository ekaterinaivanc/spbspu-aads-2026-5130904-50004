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

      Entry(const Key& k, const Value& v):
       key(k),
       value(v),
       state(OCCUPIED)
      {}

    };

    MyVector< Entry > table;
    size_t count;
    Hash hasher;
    Equal equal;

    size_t hash1(const Key& key) const
    {
      return hasher(key) % table.size();
    }

    size_t hash2(const Key& key) const
    {
      return 1 + (hasher(key) % (table.size() - 1));
    }

    size_t findIndex(const Key& key) const
    {
      size_t idx = hash1(key);
      size_t step = hash2(key);
      size_t start = idx;
      size_t i = 0;
      size_t size = table.size();

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
      if (n <= 2) {
        return 2;
      }
      if (n % 2 == 0) {
        ++n;
      }
      while (!isPrime(n)) {
        n += 2;
      }
      return n;
    }

    void rehash()
    {
      size_t newSize = nextPrime(table.size() * 2);
      DoubleHashTable newTable(newSize);
      newTable.hasher = hasher;
      newTable.equal = equal;
      for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].state == OCCUPIED) {
          newTable.insert(table[i].key, table[i].value);
        }
      }
      swap(newTable);
    }

  public:
    DoubleHashTable(size_t initialSize = DEFAULT_TABLE_SIZE):
     count(0)
    {
      size_t size = nextPrime(initialSize);
      for (size_t i = 0; i < size; ++i) {
        table.push_back(Entry());
      }
    }

    DoubleHashTable(const DoubleHashTable& other):
     count(other.count),
     hasher(other.hasher),
     equal(other.equal)
    {
      for (size_t i = 0; i < other.table.size(); ++i) {
        table.push_back(other.table[i]);
      }
    }

    void swap(DoubleHashTable& other)
    {
      std::swap(table, other.table);
      std::swap(count, other.count);
      std::swap(hasher, other.hasher);
      std::swap(equal, other.equal);
    }

    DoubleHashTable& operator=(const DoubleHashTable& other)
    {
      if (this != std::addressof(other)) {
        DoubleHashTable tmp(other);
        swap(tmp);
      }
      return *this;
    }

    bool insert(const Key& key, const Value& value)
    {
      if (findIndex(key) != static_cast< size_t >(-1)) {
        return false;
      }
      if (static_cast< double >(count) / table.size() > LOAD_FACTOR_LIMIT) {
        rehash();
      }

      size_t idx = hash1(key);
      size_t step = hash2(key);
      size_t start = idx;
      size_t i = 0;
      size_t size = table.size();
      while (table[idx].state == OCCUPIED && i < size) {
        ++i;
        idx = (start + i * step) % size;
      }
      table[idx] = Entry(key, value);
      ++count;
      return true;
    }

    struct iterator
    {
      Entry* ptr;
      Entry* endPtr;

      iterator(Entry* p = nullptr, Entry* end = nullptr):
       ptr(p),
       endPtr(end)
      {
        while (ptr != endPtr && (ptr->state == EMPTY || ptr->state == DELETED)) {
          ++ptr;
        }
      }

      Entry& operator*()
      {
        return *ptr;
      }

      Entry* operator->()
      {
        return ptr;
      }

      iterator& operator++()
      {
        ++ptr;
        while (ptr != endPtr && (ptr->state == EMPTY || ptr->state == DELETED)) {
          ++ptr;
        }
        return *this;
      }

      iterator operator++(int)
      {
        iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator==(const iterator& other) const
      {
        return ptr == other.ptr;
      }

      bool operator!=(const iterator& other) const
      {
        return ptr != other.ptr;
      }
    };

    struct const_iterator
    {
      const Entry* ptr;
      const Entry* endPtr;

      const_iterator(const Entry* p = nullptr, const Entry* end = nullptr):
       ptr(p),
       endPtr(end)
      {}

      const Entry& operator*() const
      {
        return *ptr;
      }

      const Entry* operator->() const
      {
        return ptr;
      }

      const_iterator& operator++()
      {
        ++ptr;
        while (ptr != endPtr && (ptr->state == EMPTY || ptr->state == DELETED)) {
          ++ptr;
        }
        return *this;
      }

      const_iterator operator++(int)
      {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator==(const const_iterator& other) const
      {
        return ptr == other.ptr;
      }

      bool operator!=(const const_iterator& other) const
      {
        return ptr != other.ptr;
      }
    };

    iterator begin()
    {
      Entry* first = table.begin();
      Entry* last = table.end();
      while (first != last && (first->state == EMPTY || first->state == DELETED)) {
        ++first;
      }
      return iterator(first, last);
    }

    iterator end()
    {
      return iterator(table.end(), table.end());
    }

    const_iterator begin() const
    {
      const Entry* first = table.begin();
      const Entry* last = table.end();
      while (first != last && (first->state == EMPTY || first->state == DELETED)) {
        ++first;
      }
      return const_iterator(first, last);
    }

    const_iterator end() const
    {
      return const_iterator(table.end(), table.end());
    }

    iterator find(const Key& key)
    {
      size_t idx = findIndex(key);
      if (idx != static_cast< size_t >(-1)) {
        auto it = table.begin();
        for (size_t i = 0; i < idx; ++i) {
          ++it;
        }
        return iterator(it);
      }
      return end();
    }

    const_iterator find(const Key& key) const
    {
      size_t idx = findIndex(key);
      if (idx != static_cast< size_t >(-1)) {
        auto it = table.begin();
        for (size_t i = 0; i < idx; ++i) {
          ++it;
        }
        return const_iterator(it);
      }
      return end();
    }

    bool findExists(const Key& key) const
    {
      return findIndex(key) != static_cast< size_t >(-1);
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
      for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].state == OCCUPIED) {
          keys.push_back(table[i].key);
        }
      }
    }

    size_t getCount() const
    {
      return count;
    }

    size_t getSize() const
    {
      return table.size();
    }

    void clear()
    {
      table.clear();
      size_t size = nextPrime(DEFAULT_TABLE_SIZE);
      for (size_t i = 0; i < size; ++i) {
        table.push_back(Entry());
      }
      count = 0;
    }
  };
}

#endif
