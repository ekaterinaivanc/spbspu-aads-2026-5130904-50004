#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"

namespace ivantsova {

  template< class T >
  class Queue
  {
  private:
    List< T > data_;

  public:
    void push(const T& value);
    T pop();
    T& front();
    const T& front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  };

  template< class T >
  void Queue< T >::push(const T& value)
  {
    data_.push_back(value);
  }

  template< class T >
  T Queue< T >::pop()
  {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    T value = std::move(data_.front());
    data_.pop_front();
    return value;
  }

  template< class T >
  T& Queue< T >::front()
  {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    return data_.front();
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    return data_.front();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T >
  void Queue< T >::clear()
  {
    data_.clear();
  }
}

#endif
