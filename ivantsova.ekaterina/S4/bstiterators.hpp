#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP

#include "treenode.hpp"

namespace ivantsova {
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    using Node = TreeNode< Key, Value >;

    explicit BSTIterator(Node *node = nullptr);
    std::pair< const Key, Value > &operator*() const;
    std::pair< const Key, Value > *operator->() const;
    BSTIterator &operator++();
    BSTIterator operator++(int);
    BSTIterator &operator--();
    BSTIterator operator--(int);
    bool operator==(const BSTIterator &other) const;
    bool operator!=(const BSTIterator &other) const;

    template< class K, class V, class C >
    friend class BSTree;

  private:
    Node *node_;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    using Node = TreeNode< Key, Value >;

    explicit BSTConstIterator(const Node *node = nullptr);
    const std::pair< const Key, Value > &operator*() const;
    const std::pair< const Key, Value > *operator->() const;
    BSTConstIterator &operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator &operator--();
    BSTConstIterator operator--(int);
    bool operator==(const BSTConstIterator &other) const;
    bool operator!=(const BSTConstIterator &other) const;

    template< class K, class V, class C >
    friend class BSTree;

  private:
    const Node *node_;
  };
}

template< class Key, class Value >
ivantsova::BSTIterator< Key, Value >::BSTIterator(Node *node):
 node_(node)
{}

template< class Key, class Value >
std::pair< const Key, Value > &ivantsova::BSTIterator< Key, Value >::operator*() const
{
  return node_->data;
}

template< class Key, class Value >
std::pair< const Key, Value > *ivantsova::BSTIterator< Key, Value >::operator->() const
{
  return &node_->data;
}

template< class Key, class Value >
ivantsova::BSTIterator< Key, Value > &ivantsova::BSTIterator< Key, Value >::operator++()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->right_ != nullptr) {
    node_ = node_->right_;
    while (node_->left_ != nullptr) {
      node_ = node_->left_;
    }
  } else {
    Node *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->right_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
ivantsova::BSTIterator< Key, Value > ivantsova::BSTIterator< Key, Value >::operator++(int)
{
  BSTIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value >
ivantsova::BSTIterator< Key, Value > &ivantsova::BSTIterator< Key, Value >::operator--()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->left_ != nullptr) {
    node_ = node_->left_;
    while (node_->right_ != nullptr) {
      node_ = node_->right_;
    }
  } else {
    Node *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->left_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
ivantsova::BSTIterator< Key, Value > ivantsova::BSTIterator< Key, Value >::operator--(int)
{
  BSTIterator tmp = *this;
  --(*this);
  return tmp;
}

template< class Key, class Value >
bool ivantsova::BSTIterator< Key, Value >::operator==(const BSTIterator &other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool ivantsova::BSTIterator< Key, Value >::operator!=(const BSTIterator &other) const
{
  return node_ != other.node_;
}

template< class Key, class Value >
ivantsova::BSTConstIterator< Key, Value >::BSTConstIterator(const Node *node):
 node_(node)
{}

template< class Key, class Value >
const std::pair< const Key, Value > &ivantsova::BSTConstIterator< Key, Value >::operator*() const
{
  return node_->data;
}

template< class Key, class Value >
const std::pair< const Key, Value > *ivantsova::BSTConstIterator< Key, Value >::operator->() const
{
  return &node_->data;
}

template< class Key, class Value >
ivantsova::BSTConstIterator< Key, Value > &ivantsova::BSTConstIterator< Key, Value >::operator++()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->right_ != nullptr) {
    node_ = node_->right_;
    while (node_->left_ != nullptr) {
      node_ = node_->left_;
    }
  } else {
    const Node *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->right_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
ivantsova::BSTConstIterator< Key, Value > ivantsova::BSTConstIterator< Key, Value >::operator++(int)
{
  BSTConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value >
ivantsova::BSTConstIterator< Key, Value > &ivantsova::BSTConstIterator< Key, Value >::operator--()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->left_ != nullptr) {
    node_ = node_->left_;
    while (node_->right_ != nullptr) {
      node_ = node_->right_;
    }
  } else {
    const Node *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->left_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
ivantsova::BSTConstIterator< Key, Value > ivantsova::BSTConstIterator< Key, Value >::operator--(int)
{
  BSTConstIterator tmp = *this;
  --(*this);
  return tmp;
}

template< class Key, class Value >
bool ivantsova::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator &other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool ivantsova::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator &other) const
{
  return node_ != other.node_;
}

#endif
