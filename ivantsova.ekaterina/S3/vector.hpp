#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>

namespace ivantsova {
  template< class T > class Vector;

  template< class T >
  class Iterator {
  public:
    Iterator(Vector< T >& vec, size_t idx);
    Iterator& operator+=(size_t n);
    Iterator& operator-=(size_t n);
    T& operator*() const;
    bool operator==(const Iterator<T>& other) const;
    bool operator!=(const Iterator<T>& other) const;
    Iterator operator+(size_t n) const;
    Iterator operator-(size_t n) const;
    Vector< T >& vector;
    size_t id;
  };

  template< class T >
  class CIterator {
  public:
    CIterator(const Vector< T >& vec, size_t idx);
    CIterator& operator+=(size_t n);
    CIterator& operator-=(size_t n);
    const T& operator*() const;
    bool operator==(const CIterator< T >& other) const;
    bool operator!=(const CIterator< T >& other) const;
    CIterator operator+(size_t n) const;
    CIterator operator-(size_t n) const;
    const Vector< T >& vector;
    size_t id;
  };

  template< class T >
  class Vector {
   public:
    Vector();
    ~Vector();
    explicit Vector(size_t size);
    Vector(size_t size, const T& value);

    Vector(const Vector< T >& rhs);
    Vector(Vector< T >&& rhs) noexcept;

    Vector< T >& operator=(const Vector< T >& rhs);
    Vector< T >& operator=(Vector< T >&& rhs) noexcept;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void pushBack(const T&);
    void popBack();
    void pushFront(const T& v);

    void swap(Vector< T >& rhs) noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    void insert(size_t id, const T& t);
    void insert(size_t id, const Vector< T >& rhs, size_t beg, size_t end);
    void erase(size_t id);
    void erase(size_t beg, size_t end);

    Iterator< T > begin();
    Iterator< T > end();
    CIterator< T > begin() const;
    CIterator< T > end() const;
    CIterator< T > cbegin() const;
    CIterator< T > cend() const;

    Iterator< T > insert(Iterator< T > pos, const T& value);
    Iterator< T > insert(Iterator< T > pos, CIterator< T > beg, CIterator< T > end);
    Iterator< T > insert(Iterator< T > pos, size_t count, const T& value);

    Iterator< T > erase(Iterator< T > pos);
    Iterator< T > erase(Iterator< T > beg, Iterator< T > end);
    Iterator< T > erase(CIterator< T > beg, CIterator< T > end);

   private:
    T* data_;
    size_t size_, capacity_;
  };
}

template< class T >
ivantsova::Vector< T >::Vector() :
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
ivantsova::Vector< T >::~Vector() {
  delete [] data_;
}

template< class T >
ivantsova::Vector< T >::Vector(size_t size) :
  data_(size ? new T[size] : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
ivantsova::Vector< T >::Vector(size_t size, const T& value):
  Vector(size)
{
  for (size_t i = 0; i < size; ++i)
  {
    data_[i] = value;
  }
}

template< class T >
ivantsova::Vector< T >::Vector(const Vector< T >& rhs):
  Vector(rhs.getSize())
  {
    for (size_t i = 0; i < rhs.getSize(); ++i) {
      data_[i] = rhs.data_[i];
    }
  }

template< class T >
ivantsova::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  Vector() {
    swap(rhs);
  }

template< class T >
ivantsova::Vector< T >& ivantsova::Vector< T >::operator=(const Vector< T >& rhs) {
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template< class T >
ivantsova::Vector< T >& ivantsova::Vector< T >::operator=(Vector< T >&& rhs) noexcept {
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
T& ivantsova::Vector< T >::operator[](size_t id) noexcept {
  return const_cast< T& >((*static_cast< const Vector< T >* >(this))[id]);
}

template< class T >
const T& ivantsova::Vector< T >::operator[](size_t id) const noexcept {
  return data_[id];
}

template< class T >
bool ivantsova::Vector< T >::isEmpty() const noexcept {
  return !size_;
}

template< class T >
size_t ivantsova::Vector< T >::getSize() const noexcept {
  return size_;
}

template< class T >
size_t ivantsova::Vector< T >::getCapacity() const noexcept {
  return capacity_;
}

template< class T >
void ivantsova::Vector< T >::pushBack(const T& rhs) {
  if (size_ >= capacity_) {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    T* newData = new T[newCapacity];
    try {
      for (size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
      }
    } catch (...) {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
  data_[size_++] = rhs;
}

template< class T >
void ivantsova::Vector< T >::popBack() {
  if (size_) {
    --size_;
  }
}

template< class T >
void ivantsova::Vector< T >::pushFront(const T& t) {
  Vector< T > v(getSize() + 1);
  v[0] = t;
  for (size_t i = 1; i < v.getSize(); ++i) {
    v[i] = (*this)[i - 1];
  }
  swap(v);
}

template< class T >
void ivantsova::Vector< T >::swap(Vector< T >& rhs) noexcept {
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
T& ivantsova::Vector< T >::at(size_t id) {
  if (id >= getSize()) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[id];
}

template< class T >
const T& ivantsova::Vector< T >::at(size_t id) const {
  if (id >= getSize()) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[id];
}

template< class T >
void ivantsova::Vector< T >::insert(size_t id, const T& t) {
  if (id > size_) {
    throw std::out_of_range("Index out of range");
  }
  Vector< T > temp;
  for (size_t i = 0; i < id; ++i) {
    temp.pushBack(data_[i]);
  }
  temp.pushBack(t);
  for (size_t i = id; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template< class T >
void ivantsova::Vector< T >::insert(size_t id, const Vector< T >& rhs, size_t beg, size_t end) {
  if (id > size_) {
    throw std::out_of_range("Index out of range");
  }
  if (beg > end || end > rhs.getSize()) {
    throw std::out_of_range("Range is invalid");
  }
  Vector< T > temp;
  for (size_t i = 0; i < id; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = beg; i < end; ++i) {
    temp.pushBack(rhs[i]);
  }
  for (size_t i = id; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template< class T >
void ivantsova::Vector< T >::erase(size_t id) {
  if (id >= size_) {
    throw std::out_of_range("Index out of range");
  }
  Vector< T > temp;
  for (size_t i = 0; i < id; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = id + 1; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template< class T >
void ivantsova::Vector< T >::erase(size_t beg, size_t end) {
  if (beg > end || end > size_) {
    throw std::out_of_range("Range is invalid");
  }
  Vector< T > temp;
  for (size_t i = 0; i < beg; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = end; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template< class T >
ivantsova::Iterator< T >::Iterator(ivantsova::Vector< T >& vec, size_t idx) :
  vector(vec), id(idx)
{}

template< class T >
ivantsova::Iterator< T >& ivantsova::Iterator< T >::operator+=(size_t n) {
  id += n;
  return *this;
}

template< class T >
ivantsova::Iterator< T >& ivantsova::Iterator< T >::operator-=(size_t n) {
  id -= n;
  return *this;
}

template< class T >
T& ivantsova::Iterator< T >::operator*() const {
  return vector[id];
}

template< class T >
bool ivantsova::Iterator< T >::operator==(const Iterator< T >& other) const {
  return id == other.id;
}

template< class T >
bool ivantsova::Iterator< T >::operator!=(const Iterator< T >& other) const {
  return id != other.id;
}

template< class T >
ivantsova::Iterator< T > ivantsova::Iterator< T >::operator+(size_t n) const {
  return ivantsova::Iterator< T >(vector, id + n);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Iterator< T >::operator-(size_t n) const {
  return ivantsova::Iterator< T >(vector, id - n);
}

template< class T >
ivantsova::CIterator< T >::CIterator(const ivantsova::Vector< T >& vec, size_t idx) :
  vector(vec), id(idx)
{}

template< class T >
ivantsova::CIterator< T >& ivantsova::CIterator< T >::operator+=(size_t n) {
  id += n;
  return *this;
}

template< class T >
ivantsova::CIterator< T >& ivantsova::CIterator< T >::operator-=(size_t n) {
  id -= n;
  return *this;
}

template< class T >
const T& ivantsova::CIterator< T >::operator*() const {
  return vector[id];
}

template< class T >
bool ivantsova::CIterator< T >::operator==(const CIterator< T >& other) const {
  return id == other.id;
}

template< class T >
bool ivantsova::CIterator< T >::operator!=(const CIterator< T >& other) const {
  return id != other.id;
}

template< class T >
ivantsova::CIterator< T > ivantsova::CIterator< T >::operator+(size_t n) const {
  return ivantsova::CIterator< T >(vector, id + n);
}

template< class T >
ivantsova::CIterator< T > ivantsova::CIterator< T >::operator-(size_t n) const {
  return ivantsova::CIterator< T >(vector, id - n);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Vector< T >::begin() {
  return Iterator< T >(*this, 0);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Vector< T >::end() {
  return Iterator< T >(*this, size_);
}

template< class T >
ivantsova::CIterator< T > ivantsova::Vector< T >::begin() const {
  return CIterator<T>(*this, 0);
}

template< class T >
ivantsova::CIterator< T > ivantsova::Vector< T >::end() const {
  return CIterator< T >(*this, size_);
}

template< class T >
ivantsova::CIterator< T > ivantsova::Vector< T >::cbegin() const {
  return CIterator< T >(*this, 0);
}

template< class T >
ivantsova::CIterator< T > ivantsova::Vector< T >::cend() const {
  return CIterator< T >(*this, size_);
}

template< class T >
ivantsova::Iterator<  T> ivantsova::Vector< T >::insert(Iterator< T > pos, const T& value) {
  size_t index = pos.id;
  insert(index, value);
  return Iterator< T >(*this, index);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Vector< T >::insert(Iterator< T > pos, CIterator< T > beg, CIterator< T > end) {
  if (beg.id == end.id) {
    return pos;
  }
  size_t index = pos.id;
  Vector< T > temp;
  for (size_t i = 0; i < index; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = beg.id; i < end.id; ++i) {
    temp.pushBack(beg.vector[i]);
  }
  for (size_t i = index; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
  return Iterator< T >(*this, index);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Vector< T >::insert(Iterator< T > pos, size_t count, const T& value) {
  if (count == 0) {
    return pos;
  }
  size_t index = pos.id;
  Vector< T >  temp;
  for (size_t i = 0; i < index; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = 0; i < count; ++i) {
    temp.pushBack(value);
  }
  for (size_t i = index; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
  return Iterator< T >(*this, index);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Vector< T >::erase(Iterator< T > pos) {
  size_t index = pos.id;
  erase(index);
  return Iterator< T >(*this, index);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Vector< T >::erase(Iterator< T > beg, Iterator< T > end) {
  if (beg.id == end.id) {
    return beg;
  }
  size_t b = beg.id;
  size_t e = end.id;
  erase(b, e);
  return Iterator< T >(*this, b);
}

template< class T >
ivantsova::Iterator< T > ivantsova::Vector< T >::erase(CIterator< T > beg, CIterator< T > end) {
  if (beg.id == end.id) {
    return Iterator< T >(*this, beg.id);
  }
  size_t b = beg.id;
  size_t e = end.id;
  erase(b, e);
  return Iterator< T >(*this, b);
}

#endif
