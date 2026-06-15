#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include <iterator>
#include <memory>
#include "node.hpp"

namespace ivantsova
{
  template< class T > class List;

  template< class T > class LIter:
    public std::iterator< std::bidirectional_iterator_tag, T, std::ptrdiff_t, T*, T& >
  {
    friend class List< T >;

  public:
    LIter() noexcept:
     ptr(nullptr),
     head(nullptr)
    {}
    LIter(const LIter&) noexcept = default;
    LIter(LIter&&) noexcept = default;
    ~LIter() = default;
    LIter& operator=(const LIter&) noexcept = default;
    LIter& operator=(LIter&&) noexcept = default;

    T& operator*() const noexcept
    {
      return ptr->data;
    }

    T* operator->() const noexcept
    {
      return std::addressof(ptr->data);
    }

    LIter& operator++() noexcept
    {
      ptr = ptr->next;
      if (ptr == head)
      {
        ptr = nullptr;
      }
      return *this;
    }

    LIter& operator--() noexcept
    {
      if (ptr == nullptr)
      {
        if (head == nullptr)
        {
          return *this;
        }
        ptr = head->prev;
      }
      else
      {
        ptr = ptr->prev;
      }
      return *this;
    }

    bool operator==(const LIter& other) const noexcept
    {
      return ptr == other.ptr;
    }

    bool operator!=(const LIter& other) const noexcept
    {
      return ptr != other.ptr;
    }

  private:
    Node< T >* ptr;
    Node< T >* head;
    explicit LIter(Node< T >* p, Node< T >* h) noexcept:
     ptr(p),
     head(h)
    {}
  };

  template< class T > class LCIter:
    public std::iterator< std::bidirectional_iterator_tag, const T, std::ptrdiff_t, const T*, const T& >
  {
    friend class ivantsova::List< T >;

  public:
    LCIter() noexcept:
     ptr(nullptr),
     head(nullptr)
    {}
    LCIter(const LCIter&) noexcept = default;
    LCIter(LCIter&&) noexcept = default;
    ~LCIter() = default;
    LCIter& operator=(const LCIter&) noexcept = default;
    LCIter& operator=(LCIter&&) noexcept = default;

    const T& operator*() const noexcept
    {
      return ptr->data;
    }

    const T* operator->() const noexcept
    {
      return std::addressof(ptr->data);
    }

    LCIter& operator++() noexcept
    {
      ptr = ptr->next;
      if (ptr == head)
      {
        ptr = nullptr;
      }
      return *this;
    }

    LCIter& operator--() noexcept
    {
      if (ptr == nullptr)
      {
        if (head == nullptr)
        {
          return *this;
        }
        ptr = head->prev;
      }
      else
      {
        ptr = ptr->prev;
      }
      return *this;
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return ptr == other.ptr;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return ptr != other.ptr;
    }
  private:
    const Node< T >* ptr;
    const Node< T >* head;
    explicit LCIter(const Node< T >* p, const Node< T >* h) noexcept:
     ptr(p),
     head(h)
    {}
  };
}
#endif
