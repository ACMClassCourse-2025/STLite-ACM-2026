#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <climits>
#include <cstddef>

#include "exceptions.hpp"

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template <typename T>
class vector
{
 public:
  /**
   * TODO
   * a type for actions of the elements of a vector, and you should write
   *   a class named const_iterator with same interfaces.
   */
  /**
   * you can see RandomAccessIterator at CppReference for help.
   */
  class const_iterator;
  class iterator
  {
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a
    // type. For instance, for an iterator, iterator::value_type is the type
    // that the iterator points to. STL algorithms and containers may use
    // these type_traits (e.g. the following typedef) to work properly. In
    // particular, without the following code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type:
    // https://blog.csdn.net/u014299153/article/details/72419713 About
    // iterator_category: https://en.cppreference.com/w/cpp/iterator
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

   private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    T *ptr_;
    vector *v_;

   public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    iterator()
    {
      ptr_ = nullptr;
      v_ = nullptr;
    }
    iterator(T *ptr, vector *v)
    {
      ptr_ = ptr;
      v_ = v;
    }
    iterator operator+(const int &n) const
    {
      return iterator(ptr_ + n, v_);
      // TODO
    }
    iterator operator-(const int &n) const
    {
      return iterator(ptr_ - n, v_);
      // TODO
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const
    {
      if (v_ != rhs.v_)
      {
        throw invalid_iterator();
      }
      return ptr_ - rhs.ptr_;
      // TODO
    }
    iterator &operator+=(const int &n)
    {
      ptr_ += n;
      return *this;
      // TODO
    }
    iterator &operator-=(const int &n)
    {
      ptr_ -= n;
      return *this;
      // TODO
    }
    /**
     * TODO iter++
     */
    iterator operator++(int)
    {
      ptr_++;
      return iterator(ptr_ - 1, v_);
    }
    /**
     * TODO ++iter
     */
    iterator &operator++()
    {
      ptr_++;
      return *this;
    }
    /**
     * TODO iter--
     */
    iterator operator--(int)
    {
      ptr_--;
      return iterator(ptr_ + 1, v_);
    }
    /**
     * TODO --iter
     */
    iterator &operator--()
    {
      ptr_--;
      return *this;
    }
    /**
     * TODO *it
     */
    T &operator*() const
    {
      return *ptr_;
    }
    /**
     * a operator to check whether two iterators are same (pointing to the
     * same memory address).
     */
    bool operator==(const iterator &rhs) const
    {
      if (ptr_ == rhs.ptr_)
      {
        return true;
      }
      return false;
    }
    bool operator==(const const_iterator &rhs) const
    {
      if (ptr_ == rhs.ptr_)
      {
        return true;
      }
      return false;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const
    {
      return !(*this == rhs);
    }
    bool operator!=(const const_iterator &rhs) const
    {
      return !(*this == rhs);
    }
  };
  /**
   * TODO
   * has same function as iterator, just for a const object.
   */
  class const_iterator
  {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

   private:
    T *ptr_;
    const vector *v_;

   public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    const_iterator()
    {
      ptr_ = nullptr;
      v_ = nullptr;
    }
    const_iterator(T *ptr, const vector *v)
    {
      ptr_ = ptr;
      v_ = v;
    }
    const_iterator operator+(const int &n) const
    {
      return const_iterator(ptr_ + n, v_);
      // TODO
    }
    const_iterator operator-(const int &n) const
    {
      return const_iterator(ptr_ - n, v_);
      // TODO
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const const_iterator &rhs) const
    {
      if (v_ != rhs.v_)
      {
        throw invalid_iterator();
      }
      return ptr_ - rhs.ptr_;
      // TODO
    }
    const_iterator &operator+=(const int &n)
    {
      ptr_ += n;
      return *this;
      // TODO
    }
    const_iterator &operator-=(const int &n)
    {
      ptr_ -= n;
      return *this;
      // TODO
    }
    /**
     * TODO iter++
     */
    const_iterator operator++(int)
    {
      ptr_++;
      return const_iterator(ptr_ - 1, v_);
    }
    /**
     * TODO ++iter
     */
    const_iterator &operator++()
    {
      ptr_++;
      return *this;
    }
    /**
     * TODO iter--
     */
    const_iterator operator--(int)
    {
      ptr_--;
      return const_iterator(ptr_ + 1, v_);
    }
    /**
     * TODO --iter
     */
    const_iterator &operator--()
    {
      ptr_--;
      return *this;
    }
    /**
     * TODO *it
     */
    T operator*() const
    {
      return *ptr_;
    }
    /**
     * a operator to check whether two iterators are same (pointing to the
     * same memory address).
     */
    bool operator==(const iterator &rhs) const
    {
      if (ptr_ == rhs.ptr_)
      {
        return true;
      }
      return false;
    }
    bool operator==(const const_iterator &rhs) const
    {
      if (ptr_ == rhs.ptr_)
      {
        return true;
      }
      return false;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const
    {
      return !(*this == rhs);
    }
    bool operator!=(const const_iterator &rhs) const
    {
      return !(*this == rhs);
    }
    /*TODO*/
  };
  /**
   * TODO Constructs
   * At least two: default constructor, copy constructor
   */
  vector()
  {
    data_ = nullptr;
    size_ = 0;
    length_ = 0;
  }
  vector(const vector &other)
  {
    data_ = static_cast<T *>(operator new(sizeof(T) * other.length_));
    for (int i = 0; i + 1 <= other.size_; i++)
    {
      new (data_ + i) T(other.data_[i]);
    }
    size_ = other.size_;
    length_ = other.length_;
  }
  /**
   * TODO Destructor
   */
  ~vector()
  {
    for (int i = 0; i + 1 <= size_; i++)
    {
      data_[i].~T();
    }
    operator delete(data_);
  }
  /**
   * TODO Assignment operator
   */
  vector &operator=(const vector &other)
  {
    if (this == &other)
    {
      return *this;
    }
    for (int i = 0; i + 1 <= size_; i++)
    {
      data_[i].~T();
    }
    operator delete(data_);
    data_ = static_cast<T *>(operator new(sizeof(T) * other.length_));
    for (int i = 0; i + 1 <= other.size_; i++)
    {
      new (data_ + i) T(other.data_[i]);
    }
    size_ = other.size_;
    length_ = other.length_;
    return *this;
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   */
  T &at(const size_t &pos)
  {
    if (pos >= size_)
    {
      throw index_out_of_bound();
    }
    return data_[pos];
  }
  const T &at(const size_t &pos) const
  {
    if (pos >= size_)
    {
      throw index_out_of_bound();
    }
    return data_[pos];
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   * !!! Pay attentions
   *   In STL this operator does not check the boundary but I want you to do.
   */
  T &operator[](const size_t &pos)
  {
    if (pos >= size_)
    {
      throw index_out_of_bound();
    }
    return data_[pos];
  }
  const T &operator[](const size_t &pos) const
  {
    if (pos >= size_)
    {
      throw index_out_of_bound();
    }
    return data_[pos];
  }
  /**
   * access the first element.
   * throw container_is_empty if size == 0
   */
  const T &front() const
  {
    if (size_ == 0)
    {
      throw container_is_empty();
    }
    return data_[0];
  }
  /**
   * access the last element.
   * throw container_is_empty if size == 0
   */
  const T &back() const
  {
    if (size_ == 0)
    {
      throw container_is_empty();
    }
    return data_[size_ - 1];
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin()
  {
    return iterator(data_, this);
  }
  const_iterator begin() const
  {
    return const_iterator(data_, this);
  }
  const_iterator cbegin() const
  {
    return const_iterator(data_, this);
  }
  /**
   * returns an iterator to the end.
   */
  iterator end()
  {
    return iterator(data_ + size_, this);
  }
  const_iterator end() const
  {
    return const_iterator(data_ + size_, this);
  }
  const_iterator cend() const
  {
    return const_iterator(data_ + size_, this);
  }
  /**
   * checks whether the container is empty
   */
  bool empty() const
  {
    if (size_ == 0)
    {
      return true;
    }
    return false;
  }
  /**
   * returns the number of elements
   */
  size_t size() const
  {
    return size_;
  }
  /**
   * clears the contents
   */
  void clear()
  {
    for (int i = 0; i + 1 <= size_; i++)
    {
      data_[i].~T();
    }
		size_ = 0;
  }
  /**
   * inserts value before pos
   * returns an iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, const T &value)
  {
    int ind = pos - begin();
    if (size_ == length_)
    {
      length_ = (length_ == 0) ? 1 : (length_ * 2);
      auto new_data = static_cast<T *>(operator new(sizeof(T) * length_));
      for (int i = 0; i + 1 <= size_; i++)
      {
        new (new_data + i) T(data_[i]);
        data_[i].~T();
      }
      operator delete(data_);
      data_ = new_data;
      new_data = nullptr;
    }
    if (ind < size_)
    {
      new (data_ + size_) T(data_[size_ - 1]);
      for (int i = size_ - 1; i >= ind + 1; i--)
      {
        data_[i] = data_[i - 1];
      }
			data_[ind] = value;
    }
		else
		{
			new (data_ + size_) T (value); 
		}
		size_++;
    return iterator(data_ + ind, this);
  }
  /**
   * inserts value at index ind.
   * after inserting, this->at(ind) == value
   * returns an iterator pointing to the inserted value.
   * throw index_out_of_bound if ind > size (in this situation ind can be size
   * because after inserting the size will increase 1.)
   */
  iterator insert(const size_t &ind, const T &value)
  {
    if (ind > size_)
    {
      throw index_out_of_bound();
    }
    if (size_ == length_)
    {
      length_ = (length_ == 0) ? 1 : (length_ * 2);
      auto new_data = static_cast<T *>(operator new(sizeof(T) * length_));
      for (int i = 0; i + 1 <= size_; i++)
      {
        new (new_data + i) T(data_[i]);
        data_[i].~T();
      }
      operator delete(data_);
      data_ = new_data;
      new_data = nullptr;
    }
    if (ind < size_)
    {
      new (data_ + size_) T(data_[size_ - 1]);
      for (int i = size_ - 1; i >= ind + 1; i--)
      {
        data_[i] = data_[i - 1];
      }
			data_[ind] = value;
    }
		else
		{
			new (data_ + size_) T (value); 
		}
		size_++;
    return iterator(data_ + ind, this);
  }
  /**
   * removes the element at pos.
   * return an iterator pointing to the following element.
   * If the iterator pos refers the last element, the end() iterator is
   * returned.
   */
  iterator erase(iterator pos)
  {
    (*pos).~T();
    for (auto it = pos; end() - it >= 2; it++)
    {
      (*it) = *(it + 1);
    }
    size_--;
    return (pos + 1);
  }
  /**
   * removes the element with index ind.
   * return an iterator pointing to the following element.
   * throw index_out_of_bound if ind >= size
   */
  iterator erase(const size_t &ind)
  {
    if (ind >= size_)
    {
      throw index_out_of_bound();
    }
    data_[ind].~T();
    for (size_t i = ind; i + 2 <= size_; i++)
    {
      data_[i] = data_[i + 1];
    }
    size_--;
    return iterator(data_ + ind, this);
  }
  /**
   * adds an element to the end.
   */
  void push_back(const T &value)
  {
    insert(size_, value);
  }
  /**
   * remove the last element from the end.
   * throw container_is_empty if size() == 0
   */
  void pop_back()
  {
    erase(size_ - 1);
  }

 private:
  T *data_;
  size_t size_;
  size_t length_;
};

}  // namespace sjtu

#endif
