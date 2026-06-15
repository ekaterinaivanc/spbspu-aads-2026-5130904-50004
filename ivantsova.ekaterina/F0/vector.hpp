#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>

namespace ivantsova {

  template< class T >
  class MyVector
  {
  private:
    T* data;
    size_t capacity;
    size_t count;
    void resize();

  public:
    MyVector();
    ~MyVector();
    MyVector(const MyVector& other);
    MyVector& operator=(const MyVector& other);

    void push_back(const T& val);
    void pop_back();
    T& operator[](int i);
    const T& operator[](int i) const;
    size_t size() const;
    bool empty() const;
    void clear();
    bool contains(const T& val) const;
    void remove(const T& val);
  };
}

template< class T >
ivantsova::MyVector< T >::MyVector():
 data(nullptr),
 capacity(0),
 count(0)
{}

template< class T >
ivantsova::MyVector< T >::~MyVector()
{
  delete[] data;
}

template< class T >
ivantsova::MyVector< T >::MyVector(const MyVector& other)
{
  count = other.count;
  capacity = other.capacity;
  if (capacity > 0) {
    data = new T[capacity];
    for (size_t i = 0; i < count; ++i) {
      data[i] = other.data[i];
    }
  } else {
    data = nullptr;
  }
}

template< class T >
ivantsova::MyVector< T >& ivantsova::MyVector< T >::operator=(const MyVector& other)
{
  if (this != &other) {
    delete[] data;
    count = other.count;
    capacity = other.capacity;
    if (capacity > 0) {
      data = new T[capacity];
      for (size_t i = 0; i < count; ++i) {
        data[i] = other.data[i];
      }
    } else {
      data = nullptr;
    }
  }
  return *this;
}

template< class T >
void ivantsova::MyVector< T >::resize()
{
  capacity = capacity == 0 ? 4 : capacity * 2;
  T* newData = new T[capacity];
  for (size_t i = 0; i < count; ++i) {
    newData[i] = data[i];
  }
  delete[] data;
  data = newData;
}

template< class T >
void ivantsova::MyVector< T >::push_back(const T& val)
{
  if (count >= capacity) {
    resize();
  }
  data[count++] = val;
}

template< class T >
void ivantsova::MyVector< T >::pop_back()
{
  if (count > 0) {
    --count;
  }
}

template< class T >
T& ivantsova::MyVector< T >::operator[](int i)
{
  return data[i];
}

template< class T >
const T& ivantsova::MyVector< T >::operator[](int i) const
{
  return data[i];
}

template< class T >
size_t ivantsova::MyVector< T >::size() const
{
  return count;
}

template< class T >
bool ivantsova::MyVector< T >::empty() const
{
  return count == 0;
}

template< class T >
void ivantsova::MyVector< T >::clear()
{
  count = 0;
}

template< class T >
bool ivantsova::MyVector< T >::contains(const T& val) const
{
  for (size_t i = 0; i < count; ++i) {
    if (data[i] == val) {
      return true;
    }
  }
  return false;
}

template< class T >
void ivantsova::MyVector< T >::remove(const T& val)
{
  for (size_t i = 0; i < count; ++i) {
    if (data[i] == val) {
      for (size_t j = i; j < count - 1; ++j) {
        data[j] = data[j + 1];
      }
      --count;
      return;
    }
  }
}

#endif
