#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <limits>
#include <memory>

namespace ivantsova
{
  template< class T > class List;

  template< class T > class LIter
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
    typename List< T >::Node* ptr;
    typename List< T >::Node* head;
    explicit LIter(typename List< T >::Node* p, typename List< T >::Node* h) noexcept :
      ptr(p),
      head(h)
    {}
  };

  template< class T > class LCIter
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
    const typename List< T >::Node* ptr;
    const typename List< T >::Node* head;
    explicit LCIter(const typename List< T >::Node* p, const typename List< T >::Node* h) noexcept:
     ptr(p),
     head(h)
    {}
  };

  template< class T > class List
  {
    friend class LIter< T >;
    friend class LCIter< T >;

  private:
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

    Node* head;
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
        Node* curr = other.head;
        Node* first = other.head;
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
      Node* tmp_head = head;
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
      push_front(const_cast< T& >(value));
    }

    void push_front(T&& value)
    {
      Node* new_node = new Node(std::move(value));
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
      Node* new_node = new Node(value);
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

    void push_back(T&& value)
    {
      Node* new_node = new Node(std::move(value));
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
        Node* toDelete = head;
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
        Node* toDelete = head->prev;
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
      return insert(pos, const_cast< T& >(value));
    }

    LIter< T > insert(LIter< T > pos, T&& value)
    {
      if (empty())
      {
        push_back(std::move(value));
        return LIter< T >(head, head);
      }
      Node* curr = pos.ptr;
      if (curr == nullptr)
      {
        push_back(std::move(value));
        return LIter< T >(head->prev, head);
      }
      Node* new_node = new Node(std::move(value), curr->prev, curr);
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
      Node* toDelete = pos.ptr;
      Node* next = toDelete->next;
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
  void sum(unsigned long long& total, unsigned long long add)
  {
    if (std::numeric_limits< unsigned long long >::max() - add < total)
    {
      throw std::overflow_error("Overflow error");
    }
    total += add;
  }
}

#endif
