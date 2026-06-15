#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <utility>
#include <stdexcept>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include "vector.hpp"
#include <list.hpp>

namespace ivantsova {

  template< typename Key, typename Value, typename Hash, typename Equal >
  class HashIter;

  template< typename Key, typename Value, typename Hash, typename Equal >
  class HashConstIter;


  template< typename T >
  struct Equal
  {
    bool operator()(const T& a, const T& b) const {
      return a == b;
    }
  };

  struct BoostSHA1Hash
  {
    size_t operator()(const std::string& s) const {
      boost::hash2::sha1_160 sha;
      sha.update(s.data(), s.size());
      return boost::hash2::get_integral_result< size_t >(sha);
    }
  };

  template< typename T >
  struct PairHash
  {
    size_t operator()(const std::pair< T, T >& p) const {
      BoostSHA1Hash hasher;
      return hasher(p.first) ^ (hasher(p.second) << 1);
    }
  };

  template< typename T >
  struct PairEqual
  {
    bool operator()(const std::pair< T, T >& a, const std::pair< T, T >& b) const {
      return a.first == b.first && a.second == b.second;
    }
  };

  template< typename Key, typename Value, typename Hash = BoostSHA1Hash, typename Equal = Equal< Key > >
  class HashTable
  {
  public:
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    explicit HashTable(size_t slots = 101);
    ~HashTable();

    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    void add(const Key& key, const Value& value);
    Value drop(const Key& key);
    Value& get(const Key& key);
    const Value& get(const Key& key) const;
    bool has(const Key& key) const noexcept;
    void rehash(size_t new_slots);

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable& other) noexcept;

    using HIter = HashIter< Key, Value, Hash, Equal >;
    using HCIter = HashConstIter< Key, Value, Hash, Equal >;
    HIter begin();
    HIter end();
    HCIter begin() const;
    HCIter end() const;
    HCIter cbegin() const;
    HCIter cend() const;

  private:
    using Bucket = List< std::pair< Key, Value > >;
    Vector< Bucket > data_;
    size_t size_;
    Hash hasher_;
    Equal equal_;

    size_t getIndex(const Key& key) const;
  };
}

template< typename Key, typename Value, typename Hash, typename Equal >
size_t ivantsova::HashTable< Key, Value, Hash, Equal >::getIndex(const Key& key) const
{
  return hasher_(key) % data_.getSize();
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
 data_(slots),
 size_(0)
{}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  clear();
}

template< typename Key, typename Value, typename Hash, typename Equal >
size_t ivantsova::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< typename Key, typename Value, typename Hash, typename Equal >
bool ivantsova::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< typename Key, typename Value, typename Hash, typename Equal >
void ivantsova::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < data_.getSize(); ++i) {
    data_[i].clear();
  }
  size_ = 0;
}

template< typename Key, typename Value, typename Hash, typename Equal >
void ivantsova::HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
{
  data_.swap(other.data_);
  std::swap(size_, other.size_);
  std::swap(hasher_, other.hasher_);
  std::swap(equal_, other.equal_);
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
 data_(other.data_.getSize()),
 size_(0),
 hasher_(other.hasher_),
 equal_(other.equal_)
{
  for (size_t i = 0; i < other.data_.getSize(); ++i) {
    for (auto it = other.data_[i].cbegin(); it != other.data_[i].cend(); ++it) {
      add((*it).first, (*it).second);
    }
  }
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
 data_(std::move(other.data_)),
 size_(other.size_),
 hasher_(std::move(other.hasher_)),
 equal_(std::move(other.equal_))
{
  other.size_ = 0;
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >& ivantsova::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
{
  if (this != &other) {
    HashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >& ivantsova::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
{
  if (this != &other) {
    swap(other);
  }
  return *this;
}

template< typename Key, typename Value, typename Hash, typename Equal >
void ivantsova::HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
{
  size_t idx = getIndex(key);
  for (auto it = data_[idx].begin(); it != data_[idx].end(); ++it) {
    if (equal_((*it).first, key)) {
      (*it).second = value;
      return;
    }
  }
  data_[idx].push_back({key, value});
  ++size_;
}

template< typename Key, typename Value, typename Hash, typename Equal >
bool ivantsova::HashTable< Key, Value, Hash, Equal >::has(const Key& key) const noexcept
{
  size_t idx = getIndex(key);
  for (auto it = data_[idx].cbegin(); it != data_[idx].cend(); ++it) {
    if (equal_((*it).first, key)) {
      return true;
    }
  }
  return false;
}

template< typename Key, typename Value, typename Hash, typename Equal >
Value& ivantsova::HashTable< Key, Value, Hash, Equal >::get(const Key& key)
{
  size_t idx = getIndex(key);
  for (auto it = data_[idx].begin(); it != data_[idx].end(); ++it) {
    if (equal_((*it).first, key)) {
      return (*it).second;
    }
  }
  throw std::out_of_range("Key not found");
}

template< typename Key, typename Value, typename Hash, typename Equal >
const Value& ivantsova::HashTable< Key, Value, Hash, Equal >::get(const Key& key) const
{
  size_t idx = getIndex(key);
  for (auto it = data_[idx].cbegin(); it != data_[idx].cend(); ++it) {
    if (equal_((*it).first, key)) {
      return (*it).second;
    }
  }
  throw std::out_of_range("Key not found");
}

template< typename Key, typename Value, typename Hash, typename Equal >
Value ivantsova::HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
{
  size_t idx = getIndex(key);
  for (auto it = data_[idx].begin(); it != data_[idx].end(); ++it) {
    if (equal_((*it).first, key)) {
      Value val = (*it).second;
      data_[idx].erase(it);
      --size_;
      return val;
    }
  }
  throw std::out_of_range("Key not found");
}

template< typename Key, typename Value, typename Hash, typename Equal >
void ivantsova::HashTable< Key, Value, Hash, Equal >::rehash(size_t new_slots)
{
  if (new_slots == 0 || new_slots == data_.getSize()) {
    return;
  }
  HashTable new_table(new_slots);
  for (size_t i = 0; i < data_.getSize(); ++i) {
    for (auto it = data_[i].begin(); it != data_[i].end(); ++it) {
      new_table.add((*it).first, (*it).second);
    }
  }
  swap(new_table);
}

template< typename Key, typename Value, typename Hash, typename Equal >
typename ivantsova::HashTable< Key, Value, Hash, Equal >::HIter ivantsova::HashTable< Key, Value, Hash, Equal >::begin()
{
  for (size_t i = 0; i < data_.getSize(); ++i) {
    if (!data_[i].empty()) {
      return HIter(this, i, data_[i].begin());
    }
  }
  return end();
}

template< typename Key, typename Value, typename Hash, typename Equal >
typename ivantsova::HashTable< Key, Value, Hash, Equal >::HIter ivantsova::HashTable< Key, Value, Hash, Equal >::end()
{
  return HIter(this, data_.getSize(), LIter< std::pair< Key, Value > >());
}

template< typename Key, typename Value, typename Hash, typename Equal >
typename ivantsova::HashTable< Key, Value, Hash, Equal >::HCIter ivantsova::HashTable< Key, Value, Hash, Equal >::begin() const
{
  for (size_t i = 0; i < data_.getSize(); ++i) {
    if (!data_[i].empty()) {
      return HCIter(this, i, data_[i].cbegin());
    }
  }
  return end();
}

template< typename Key, typename Value, typename Hash, typename Equal >
typename ivantsova::HashTable< Key, Value, Hash, Equal >::HCIter ivantsova::HashTable< Key, Value, Hash, Equal >::end() const
{
  return HCIter(this, data_.getSize(), LCIter< std::pair< Key, Value > >());
}

template< typename Key, typename Value, typename Hash, typename Equal >
typename ivantsova::HashTable< Key, Value, Hash, Equal >::HCIter ivantsova::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return begin();
}

template< typename Key, typename Value, typename Hash, typename Equal >
typename ivantsova::HashTable< Key, Value, Hash, Equal >::HCIter ivantsova::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return end();
}

#endif
