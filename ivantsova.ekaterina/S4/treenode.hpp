#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <utility>

namespace ivantsova {
  template < class Key, class Value >
  struct TreeNode
  {
    std::pair< const Key, Value > data;
    TreeNode *left_;
    TreeNode *right_;
    TreeNode *parent_;

    TreeNode(const Key &key, const Value &value, TreeNode *parent = nullptr);
  };
}

template < class Key, class Value >
ivantsova::TreeNode< Key, Value >::TreeNode(const Key &key, const Value &value,
  ivantsova::TreeNode< Key, Value > *parent):
   data(key, value),
   left_(nullptr),
   right_(nullptr),
   parent_(parent)
{}

#endif
