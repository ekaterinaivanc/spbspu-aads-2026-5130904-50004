#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace ivantsova
{
  template< class T >
  struct Node
  {
    T data;
    Node* prev;
    Node* next;

    explicit Node(const T& val, Node* p = nullptr, Node* n = nullptr):
     data(val),
     prev(p),
     next(n)
    {}

    explicit Node(T&& val, Node* p = nullptr, Node* n = nullptr):
     data(std::move(val)),
     prev(p),
     next(n)
    {}
  };
}
#endif
