#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <limits>
#include "node.hpp"
#include "iterators.hpp"

namespace ivantsova
{
  template< class T > class List
  {
    friend class LIter< T >;
    friend class LCIter< T >;

  private:
    Node< T >* head;
    size_t size_;

  public:
    List() noexcept:
     head(nullptr),
     size_(0)
    {}
    ~List()
    {
      clear();
    }
    List(const List& other):
     head(nullptr),
     size_(0)
    {
      if (other.head)
      {
        Node< T >* curr = other.head;
        Node< T >* first = other.head;
        while (true)
        {
          push_back(curr->data);
          curr = curr->next;
          if (curr == first)
          {
            break;
          }
        }
      }
    }

    List(List&& other) noexcept:
     head(other.head),
     size_(other.size_)
    {
      other.head = nullptr;
      other.size_ = 0;
    }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other)
      {
        clear();
        head = other.head;
        size_ = other.size_;
        other.head = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    void swap(List& other) noexcept
    {
      Node< T >* tmp_head = head;
      size_t tmp_size = size_;
      head = other.head;
      size_ = other.size_;
      other.head = tmp_head;
      other.size_ = tmp_size;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    T& front()
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      return head->data;
    }

    const T& front() const
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      return head->data;
    }

    T& back()
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      return head->prev->data;
    }

    const T& back() const
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      return head->prev->data;
    }

    void push_front(const T& value)
    {
      T temp = value;
      push_front(std::move(temp));
    }

    void push_front(T&& value)
    {
      Node< T >* new_node = new Node< T >(std::move(value));
      if (empty())
      {
        head = new_node;
        head->prev = head;
        head->next = head;
      }
      else
      {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
        head = new_node;
      }
      size_++;
    }

    void push_back(const T& value)
    {
      T temp = value;
      push_back(std::move(temp));
    }

    void push_back(T&& value)
    {
      Node< T >* new_node = new Node< T >(std::move(value));
      if (empty())
      {
        head = new_node;
        head->prev = head;
        head->next = head;
      }
      else
      {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
      }
      size_++;
    }

    void pop_front()
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      if (size_ == 1)
      {
        delete head;
        head = nullptr;
      }
      else
      {
        Node< T >* toDelete = head;
        head->prev->next = head->next;
        head->next->prev = head->prev;
        head = head->next;
        delete toDelete;
      }
      size_--;
    }

    void pop_back()
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      if (size_ == 1)
      {
        delete head;
        head = nullptr;
      }
      else
      {
        Node< T >* toDelete = head->prev;
        toDelete->prev->next = head;
        head->prev = toDelete->prev;
        delete toDelete;
      }
      size_--;
    }

    void clear()
    {
      while (!empty())
      {
        pop_front();
      }
    }

    LIter< T > begin() const noexcept
    {
      return LIter< T >(head, head);
    }

    LIter< T > end() const noexcept
    {
       return LIter< T >(nullptr, head);
    }

    LCIter< T > cbegin() const noexcept
    {
      return LCIter< T >(head, head);
    }

    LCIter< T > cend() const noexcept
    {
      return LCIter< T >(nullptr, head);
    }

    LIter< T > insert(LIter< T > pos, const T& value)
    {
      T temp = value;
      return insert(pos, std::move(temp));
    }

    LIter< T > insert(LIter< T > pos, T&& value)
    {
      if (empty())
      {
        push_back(std::move(value));
        return LIter< T >(head, head);
      }
      Node< T >* curr = pos.ptr;
      if (curr == nullptr)
      {
        push_back(std::move(value));
        return LIter< T >(head->prev, head);
      }
      Node< T >* new_node = new Node< T >(std::move(value), curr->prev, curr);
      curr->prev->next = new_node;
      curr->prev = new_node;
      size_++;
      return LIter< T >(new_node, head);
    }

    LIter< T > erase(LIter< T > pos)
    {
      if (empty() || pos.ptr == nullptr)
      {
        return LIter< T >();
      }
      Node< T >* toDelete = pos.ptr;
      Node< T >* next = toDelete->next;
      if (size_ == 1)
      {
        delete toDelete;
        head = nullptr;
        size_ = 0;
        return LIter< T >();
      }
      toDelete->prev->next = toDelete->next;
      toDelete->next->prev = toDelete->prev;
      if (toDelete == head)
      {
        head = head->next;
      }
      delete toDelete;
      size_--;
      return LIter< T >(next == head ? nullptr : next, head);
    }
  };
  inline void sum(unsigned long long& total, unsigned long long add)
  {
    if (std::numeric_limits< unsigned long long >::max() - add < total)
    {
      throw std::overflow_error("Overflow error");
    }
    total += add;
  }
}

#endif
