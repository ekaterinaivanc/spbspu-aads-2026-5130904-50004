#ifndef HASH_ITERS_HPP
#define HASH_ITERS_HPP

#include "hashTable.hpp"

namespace ivantsova {

  template< typename Key, typename Value, typename Hash, typename Equal >
  class HashIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;

  public:
    HashIter();
    HashIter(HashTable< Key, Value, Hash, Equal >* table, size_t bucket_index,  LIter< std::pair< Key, Value > > it);

    HashIter& operator++();
    bool operator!=(const HashIter& other) const;
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();

  private:
    HashTable< Key, Value, Hash, Equal >* table_;
    size_t bucket_index_;
    LIter< std::pair< Key, Value > > it_;
  };

  template< typename Key, typename Value, typename Hash, typename Equal >
  class HashConstIter
  {
  friend class HashTable< Key, Value, Hash, Equal >;

  public:
    HashConstIter();
    HashConstIter(const HashTable< Key, Value, Hash, Equal >* table, size_t bucket_index,
      LCIter< std::pair< Key, Value > > it);

    HashConstIter& operator++();
    bool operator!=(const HashConstIter& other) const;
    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

  private:
    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t bucket_index_;
    LCIter< std::pair< Key, Value > > it_;
  };
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashIter< Key, Value, Hash, Equal >::HashIter():
 table_(nullptr),
 bucket_index_(0),
 it_()
{}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashIter< Key, Value, Hash, Equal >::HashIter(HashTable< Key, Value, Hash, Equal >* table,
size_t bucket_index, LIter< std::pair< Key, Value > > it):
 table_(table),
 bucket_index_(bucket_index),
 it_(it)
{
  while (bucket_index_ < table_->data_.getSize() && it_ == table_->data_[bucket_index_].end()) {
    ++bucket_index_;
    if (bucket_index_ < table_->data_.getSize()) {
      it_ = table_->data_[bucket_index_].begin();
    }
  }
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashIter< Key, Value, Hash, Equal >& ivantsova::HashIter< Key, Value, Hash, Equal >::operator++()
{
  if (it_ != table_->data_[bucket_index_].end()) {
    ++it_;
  }
  while (bucket_index_ < table_->data_.getSize() && it_ == table_->data_[bucket_index_].end()) {
    ++bucket_index_;
    if (bucket_index_ < table_->data_.getSize()) {
      it_ = table_->data_[bucket_index_].begin();
    }
  }
  return *this;
}

template< typename Key, typename Value, typename Hash, typename Equal >
bool ivantsova::HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter& other) const
{
  return bucket_index_ != other.bucket_index_ || it_ != other.it_;
}

template< typename Key, typename Value, typename Hash, typename Equal >
std::pair< Key, Value >& ivantsova::HashIter< Key, Value, Hash, Equal >::operator*()
{
  return *it_;
}

template< typename Key, typename Value, typename Hash, typename Equal >
std::pair< Key, Value >* ivantsova::HashIter< Key, Value, Hash, Equal >::operator->()
{
  return &(*it_);
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
 table_(nullptr),
 bucket_index_(0),
 it_()
{}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashConstIter< Key, Value, Hash, Equal >::HashConstIter(const HashTable< Key, Value, Hash, Equal >* table,
  size_t bucket_index, LCIter< std::pair< Key, Value > > it):
   table_(table),
   bucket_index_(bucket_index),
   it_(it)
{
  while (bucket_index_ < table_->data_.getSize() && it_ == table_->data_[bucket_index_].cend()) {
    ++bucket_index_;
    if (bucket_index_ < table_->data_.getSize()) {
      it_ = table_->data_[bucket_index_].cbegin();
    }
  }
}

template< typename Key, typename Value, typename Hash, typename Equal >
ivantsova::HashConstIter< Key, Value, Hash, Equal >& ivantsova::HashConstIter< Key, Value, Hash, Equal >::operator++()
{
  if (it_ != table_->data_[bucket_index_].cend()) {
    ++it_;
  }
  while (bucket_index_ < table_->data_.getSize() && it_ == table_->data_[bucket_index_].cend()) {
    ++bucket_index_;
    if (bucket_index_ < table_->data_.getSize()) {
      it_ = table_->data_[bucket_index_].cbegin();
    }
  }
  return *this;
}

template< typename Key, typename Value, typename Hash, typename Equal >
bool ivantsova::HashConstIter< Key, Value, Hash, Equal >::operator!=(const HashConstIter& other) const
{
  return bucket_index_ != other.bucket_index_ || it_ != other.it_;
}

template< typename Key, typename Value, typename Hash, typename Equal >
const std::pair< Key, Value >& ivantsova::HashConstIter< Key, Value, Hash, Equal >::operator*() const
{
  return *it_;
}

template< typename Key, typename Value, typename Hash, typename Equal >
const std::pair< Key, Value >* ivantsova::HashConstIter< Key, Value, Hash, Equal >::operator->() const
{
  return &(*it_);
}

#endif
