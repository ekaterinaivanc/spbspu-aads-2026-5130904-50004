#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "vector.hpp"
#include "../common/list.hpp"
#include <utility>
#include <stdexcept>
#include <boost/hash2/sha1.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace ivantsova {

  template< typename T >
  struct Equal {
    bool operator()(const T& a, const T& b) const {
      return a == b;
    }
  };

  struct BoostSHA1Hash {
    size_t operator()(const std::string& s) const {
      boost::hash2::sha1_160 sha;
      sha.update(s.data(), s.size());
      return boost::hash2::get_integral_result<size_t>(sha);
    }
  };

  template< typename T >
  struct PairHash {
    size_t operator()(const std::pair<T, T>& p) const {
      BoostSHA1Hash hasher;
      return hasher(p.first) ^ (hasher(p.second) << 1);
    }
  };

  template<typename T>
  struct PairEqual {
    bool operator()(const std::pair< T, T >& a, const std::pair< T, T >& b) const {
      return a.first == b.first && a.second == b.second;
    }
  };

  template< typename Key, typename Value, typename Hash = BoostSHA1Hash, typename Equal = Equal< Key > >
  class HashTable {
  public:
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
size_t ivantsova::HashTable< Key, Value, Hash, Equal >::getIndex(const Key& key) const {
  return hasher_(key) % data_.getSize();
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(slots), size_(0)
{}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashTable< Key, Value, Hash, Equal >::~HashTable() {
  clear();
}

template< typename Key, typename Value, typename Hash, typename Equal >
size_t ivantsova::HashTable< Key, Value, Hash, Equal >::size() const noexcept {
  return size_;
}

template< typename Key, typename Value, typename Hash, typename Equal >
bool ivantsova::HashTable< Key, Value, Hash, Equal >::empty() const noexcept {
  return size_ == 0;
}

#endif
