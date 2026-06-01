#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <functional>
#include <cstddef>
#include <stdexcept>
#include "bstiterators.hpp"
#include "treenode.hpp"

namespace ivantsova {
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using Node = TreeNode< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree &other);
    ~BSTree();

    BSTree &operator=(const BSTree &other);

    bool empty() const;
    size_t size() const;
    Value get(const Key &k) const;
    void push(const Key &k, const Value &v);
    Value drop(const Key &k);

    const_iterator find(const Key &k) const;
    iterator find(const Key &k);

    void clear() noexcept;
    void swap(BSTree &other) noexcept;

    iterator begin();
    iterator end() noexcept;
    const_iterator begin() const;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height() const;
    size_t height(const_iterator it) const;

  private:
    Node *fake_root_;
    size_t size_;
    Compare comp_;

    Node *getRealRoot() const;
    Node *clone(Node *src, Node *parent);
    Node *findNode(const Key &k) const;
    Node *findNode(const Key &k);
    Node *fallLeft(Node *node) const;
    void clearImpl(Node *node) noexcept;
    size_t calcHeight(const Node *node) const;
    void removeNode(Node *node);
  };
}

template< class Key, class Value, class Compare >
ivantsova::BSTree< Key, Value, Compare >::BSTree():
 fake_root_(new Node(Key(), Value())),
 size_(0),
 comp_()
{
  fake_root_->left_ = nullptr;
  fake_root_->right_ = nullptr;
  fake_root_->parent_ = nullptr;
}

template< class Key, class Value, class Compare >
ivantsova::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
  delete fake_root_;
}

template< class Key, class Value, class Compare >
ivantsova::BSTree< Key, Value, Compare >::BSTree(const BSTree &other):
 fake_root_(new Node(Key(), Value())),
 size_(0),
 comp_(other.comp_)
{
  fake_root_->left_ = nullptr;
  fake_root_->right_ = nullptr;
  fake_root_->parent_ = nullptr;
  if (other.getRealRoot() != nullptr) {
    fake_root_->right_ = clone(other.getRealRoot(), fake_root_);
    size_ = other.size_;
  }
}

template< class Key, class Value, class Compare >
ivantsova::BSTree< Key, Value, Compare > &ivantsova::BSTree< Key, Value, Compare >::operator=(const BSTree &other)
{
  if (this != &other) {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
bool ivantsova::BSTree< Key, Value, Compare >::empty() const
{
  return size_ == 0;
}

template< class Key, class Value, class Compare >
size_t ivantsova::BSTree< Key, Value, Compare >::size() const
{
  return size_;
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::Node *
ivantsova::BSTree< Key, Value, Compare >::getRealRoot() const
{
  return fake_root_->right_;
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::Node *
ivantsova::BSTree< Key, Value, Compare >::clone(Node *src, Node *parent)
{
  if (src == nullptr) {
    return nullptr;
  }
  Node *n = new Node(src->data.first, src->data.second, parent);
  n->left_ = clone(src->left_, n);
  n->right_ = clone(src->right_, n);
  return n;
}

template< class Key, class Value, class Compare >
void ivantsova::BSTree< Key, Value, Compare >::clearImpl(Node *node) noexcept
{
  if (node == nullptr) {
    return;
  }
  clearImpl(node->left_);
  clearImpl(node->right_);
  delete node;
}

template< class Key, class Value, class Compare >
void ivantsova::BSTree< Key, Value, Compare >::clear() noexcept
{
  clearImpl(getRealRoot());
  fake_root_->right_ = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
void ivantsova::BSTree< Key, Value, Compare >::swap(BSTree &other) noexcept
{
  Node *tmp_root = fake_root_;
  fake_root_ = other.fake_root_;
  other.fake_root_ = tmp_root;

  size_t tmp_size = size_;
  size_ = other.size_;
  other.size_ = tmp_size;

  Compare tmp_comp = comp_;
  comp_ = other.comp_;
  other.comp_ = tmp_comp;
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::Node *
ivantsova::BSTree< Key, Value, Compare >::findNode(const Key &k) const
{
  Node *cur = getRealRoot();
  while (cur != nullptr) {
    if (comp_(k, cur->data.first)) {
      cur = cur->left_;
    } else if (comp_(cur->data.first, k)) {
      cur = cur->right_;
    } else {
      return cur;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::Node *
ivantsova::BSTree< Key, Value, Compare >::findNode(const Key &k)
{
  return const_cast< Node * >(static_cast< const BSTree & >(*this).findNode(k));
}

template< class Key, class Value, class Compare >
Value ivantsova::BSTree< Key, Value, Compare >::get(const Key &k) const
{
  Node *n = findNode(k);
  if (n == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return n->data.second;
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::find(const Key &k) const
{
  return const_iterator(findNode(k));
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::iterator
ivantsova::BSTree< Key, Value, Compare >::find(const Key &k)
{
  return iterator(findNode(k));
}

template< class Key, class Value, class Compare >
void ivantsova::BSTree< Key, Value, Compare >::push(const Key &k, const Value &v)
{
  if (getRealRoot() == nullptr) {
    fake_root_->right_ = new Node(k, v, fake_root_);
    ++size_;
    return;
  }
  Node *cur = getRealRoot();
  while (true) {
    if (comp_(k, cur->data.first)) {
      if (cur->left_ == nullptr) {
        cur->left_ = new Node(k, v, cur);
        ++size_;
        return;
      }
      cur = cur->left_;
    } else if (comp_(cur->data.first, k)) {
      if (cur->right_ == nullptr) {
        cur->right_ = new Node(k, v, cur);
        ++size_;
        return;
      }
      cur = cur->right_;
    } else {
      cur->data.second = v;
      return;
    }
  }
}

template< class Key, class Value, class Compare >
void ivantsova::BSTree< Key, Value, Compare >::removeNode(Node *node)
{
  if (node == nullptr || node == fake_root_) {
    return;
  }
  if (node->left_ != nullptr && node->right_ != nullptr) {
    Node *succ = fallLeft(node->right_);
    const_cast< Key & >(node->data.first) = succ->data.first;
    node->data.second = succ->data.second;
    node = succ;
  }
  Node *child = (node->left_ != nullptr) ? node->left_ : node->right_;
  if (child != nullptr) {
    child->parent_ = node->parent_;
  }
  if (node->parent_ == fake_root_) {
    fake_root_->right_ = child;
  } else if (node->parent_->left_ == node) {
    node->parent_->left_ = child;
  } else {
    node->parent_->right_ = child;
  }
  delete node;
}

template< class Key, class Value, class Compare >
Value ivantsova::BSTree< Key, Value, Compare >::drop(const Key &k)
{
  Node *node = findNode(k);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  Value res = node->data.second;
  removeNode(node);
  --size_;
  return res;
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::Node *
ivantsova::BSTree< Key, Value, Compare >::fallLeft(Node *node) const
{
  while (node != nullptr && node->left_ != nullptr) {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::iterator
ivantsova::BSTree< Key, Value, Compare >::begin()
{
  return iterator(fallLeft(getRealRoot()));
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::iterator
ivantsova::BSTree< Key, Value, Compare >::end() noexcept
{
  return iterator(nullptr);
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::begin() const
{
  return const_iterator(fallLeft(getRealRoot()));
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::end() const noexcept
{
  return const_iterator(nullptr);
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  return const_iterator(fallLeft(getRealRoot()));
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return const_iterator(nullptr);
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
{
  Node *child = const_cast< Node * >(it.node_);
  if (child == nullptr || child->right_ == nullptr || child->parent_ == fake_root_) {
    return it;
  }
  Node *parent = child->parent_;
  Node *rightChild = child->right_;
  child->right_ = rightChild->left_;
  if (rightChild->left_ != nullptr) {
    rightChild->left_->parent_ = child;
  }
  rightChild->parent_ = parent;
  if (parent == fake_root_) {
    fake_root_->right_ = rightChild;
  } else if (parent->left_ == child) {
    parent->left_ = rightChild;
  } else {
    parent->right_ = rightChild;
  }
  rightChild->left_ = child;
  child->parent_ = rightChild;
  return const_iterator(rightChild);
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
{
  Node *child = const_cast< Node * >(it.node_);
  if (child == nullptr || child->left_ == nullptr || child->parent_ == fake_root_) {
    return it;
  }
  Node *parent = child->parent_;
  Node *leftChild = child->left_;
  child->left_ = leftChild->right_;
  if (leftChild->right_ != nullptr) {
    leftChild->right_->parent_ = child;
  }
  leftChild->parent_ = parent;
  if (parent == fake_root_) {
    fake_root_->right_ = leftChild;
  } else if (parent->left_ == child) {
    parent->left_ = leftChild;
  } else {
    parent->right_ = leftChild;
  }
  leftChild->right_ = child;
  child->parent_ = leftChild;
  return const_iterator(leftChild);
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
{
  Node *node = const_cast< Node * >(it.node_);
  if (node == nullptr || node->left_ == nullptr || node->left_->right_ == nullptr) {
    return it;
  }
  rotateRight(const_iterator(node->left_));
  return rotateLeft(it);
}

template< class Key, class Value, class Compare >
typename ivantsova::BSTree< Key, Value, Compare >::const_iterator
ivantsova::BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
{
  Node *node = const_cast< Node * >(it.node_);
  if (node == nullptr || node->right_ == nullptr || node->right_->left_ == nullptr) {
    return it;
  }
  rotateLeft(const_iterator(node->right_));
  return rotateRight(it);
}

template< class Key, class Value, class Compare >
size_t ivantsova::BSTree< Key, Value, Compare >::calcHeight(const Node *node) const
{
  if (node == nullptr) {
    return 0;
  }
  size_t l = calcHeight(node->left_);
  size_t r = calcHeight(node->right_);
  return 1 + ((l > r) ? l : r);
}

template< class Key, class Value, class Compare >
size_t ivantsova::BSTree< Key, Value, Compare >::height() const
{
  return calcHeight(getRealRoot());
}

template< class Key, class Value, class Compare >
size_t ivantsova::BSTree< Key, Value, Compare >::height(const_iterator it) const
{
  return calcHeight(it.node_);
}

#endif
