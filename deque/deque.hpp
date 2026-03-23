#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include <cstddef>

#include "exceptions.hpp"

namespace sjtu
{

template <class T>
class deque
{
 public:
  static constexpr int blockSize =
      16;  // we define 16 elements as full size of a block;
  class const_iterator;
  class iterator
  {
   private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    int cur_;   // from 0 to 15
    T **node_;  // it point the pos of the node of the list
    deque *deq_;
    friend class deque<T>;
    friend class const_iterator;

   public:
    iterator() : cur_(0), node_(nullptr), deq_(nullptr)
    {
    }
    iterator(int cur, T **node, deque *deq)
    {
      cur_ = cur;
      node_ = node;
      deq_ = deq;
    }
    /**
     * return a new iterator which pointer n-next elements
     *   even if there are not enough elements, the behaviour is **undefined**.
     * as well as operator-
     */
    inline iterator operator+(const int &n) const
    {
      if (n == 0)
      {
        return *this;
      }
      int offset = cur_ + n;
      T **new_node = node_;
      int new_cur = 0;
      if (offset >= 0)
      {
        new_node += offset >> 4;
        new_cur += offset & 15;
      }
      else
      {
        int back = (-offset - 1) >> 4;
        back++;
        new_node -= back;
        new_cur = 15 - (-offset - 1) & 15;
      }
      return iterator(new_cur, new_node, deq_);
    }
    inline iterator operator-(const int &n) const
    {
      if (n == 0)
      {
        return *this;
      }
      int offset = cur_ - n;
      T **new_node = node_;
      int new_cur = 0;
      if (offset >= 0)
      {
        new_node += offset / blockSize;
        new_cur += offset % blockSize;
      }
      else
      {
        int back = (-offset - 1) / blockSize;
        back++;
        new_node -= back;
        new_cur = blockSize - 1 - (-offset - 1) % blockSize;
      }
      return iterator(new_cur, new_node, deq_);
    }
    // return th distance between two iterator,
    // if these two iterators points to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const
    {
      if (deq_ != rhs.deq_)  // from different deques
      {
        throw invalid_iterator();
      }
      return (node_ - rhs.node_) * blockSize + cur_ - rhs.cur_;
    }
    iterator operator+=(const int &n)
    {
      // TODO
      *this = *this + n;
      return *this;
    }
    iterator operator-=(const int &n)
    {
      // TODO
      *this = *this + (-n);
      return *this;
    }
    /**
     * TODO iter++
     */
    iterator operator++(int)
    {
      iterator tmp = *this;
      if (cur_ != blockSize - 1)
      {
        cur_++;
      }
      else
      {
        cur_ = 0;
        node_++;
      }
      return tmp;
    }
    /**
     * TODO ++iter
     */
    iterator &operator++()
    {
      if (cur_ != blockSize - 1)
      {
        cur_++;
      }
      else
      {
        cur_ = 0;
        node_++;
      }
      return *this;
    }
    /**
     * TODO iter--
     */
    iterator operator--(int)
    {
      iterator tmp = *this;
      if(cur_ != 0)
      {
        cur_--;
      }
      else
      {
        cur_ = blockSize - 1;
        node_--;
      }
      return tmp;
    }
    /**
     * TODO --iter
     */
    iterator &operator--()
    {
      if(cur_ != 0)
      {
        cur_--;
      }
      else
      {
        cur_ = blockSize - 1;
        node_--;
      }
      return *this;
    }
    /**
     * TODO *it
     */
    T &operator*() const
    {
      int pos = *this - deq_->start_;
      if (pos < 0 || pos >= deq_->data_size_)
      {
        throw index_out_of_bound();
      }
      return (*node_)[cur_];
    }
    /**
     * TODO it->field
     */
    T *operator->() const noexcept
    {
      return (*node_) + cur_;
    }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const
    {
      return (node_ == rhs.node_ && cur_ == rhs.cur_);
    }
    bool operator==(const const_iterator &rhs) const
    {
      return (node_ == rhs.node_ && cur_ == rhs.cur_);
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
  class const_iterator
  {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
   private:
    int cur_;         // from 0 to 15
    const T **node_;  // it point the pos of the node of the list
    const deque *deq_;
    friend class deque<T>;
    friend class iterator;
    // data members.
   public:
    const_iterator(const int cur, const T **node, const deque *deq)
    {
      cur_ = cur;
      node_ = node;
      deq_ = deq;
    }
    const_iterator() : cur_(0), node_(nullptr), deq_(nullptr)
    {
    }
    const_iterator(const iterator &other)
        : cur_(other.cur_), node_((const T **)other.node_), deq_(other.deq_)
    {
      // TODO
    }
    inline const_iterator operator+(const int &n) const
    {
      if (n == 0)
      {
        return *this;
      }
      int offset = cur_ + n;
      const T **new_node = node_;
      int new_cur = 0;
      if (offset >= 0)
      {
        new_node += offset >> 4;
        new_cur += offset & 15;
      }
      else
      {
        int back = (-offset - 1) >> 4;
        back++;
        new_node -= back;
        new_cur = 15 - (-offset - 1) % 15;
      }
      return const_iterator(new_cur, new_node, deq_);
    }
    inline const_iterator operator-(const int &n) const
    {
      if (n == 0)
      {
        return *this;
      }
      int offset = cur_ - n;
      const T **new_node = node_;
      int new_cur = 0;
      if (offset >= 0)
      {
        new_node += offset / blockSize;
        new_cur += offset % blockSize;
      }
      else
      {
        int back = (-offset - 1) / blockSize;
        back++;
        new_node -= back;
        new_cur = blockSize - 1 - (-offset - 1) % blockSize;
      }
      return const_iterator(new_cur, new_node, deq_);
    }
    int operator-(const const_iterator &rhs) const
    {
      // TODO
      if (deq_ != rhs.deq_)  // from different deques
      {
        throw invalid_iterator();
      }
      return (node_ - rhs.node_) * blockSize + cur_ - rhs.cur_;
    }
    const_iterator operator+=(const int &n)
    {
      *this = *this + n;
      return *this;
    }
    const_iterator operator-=(const int &n)
    {
      *this = *this + (-n);
      return *this;
    }
    const_iterator operator++(int)
    {
      const_iterator tmp = *this;
      if (cur_ != blockSize - 1)
      {
        cur_++;
      }
      else
      {
        cur_ = 0;
        node_++;
      }
      return tmp;
    }
    const_iterator &operator++()
    {
      if (cur_ != blockSize - 1)
      {
        cur_++;
      }
      else
      {
        cur_ = 0;
        node_++;
      }
      return *this;
    }
    const_iterator operator--(int)
    {
      const_iterator tmp = *this;
      if(cur_ != 0)
      {
        cur_--;
      }
      else
      {
        cur_ = blockSize - 1;
        node_--;
      }
      return tmp;
    }
    const_iterator &operator--()
    {
      if(cur_ != 0)
      {
        cur_--;
      }
      else
      {
        cur_ = blockSize - 1;
        node_--;
      }
      return *this;
    }
    const T &operator*() const
    {
      int pos = *this - deq_->start_;
      if (pos < 0 || pos >= deq_->data_size_)
      {
        throw index_out_of_bound();
      }
      return (*node_)[cur_];
    }
    const T *operator->() const noexcept
    {
      return (*node_) + cur_;
    }
    bool operator==(const iterator &rhs) const
    {
      return (node_ == rhs.node_ && cur_ == rhs.cur_);
    }
    bool operator==(const const_iterator &rhs) const
    {
      return (node_ == rhs.node_ && cur_ == rhs.cur_);
    }
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
   * TODO Constructors
   */
  deque()
  {
    init();
  }
  deque(const deque &other)
  {
    init();
    for (int i = 0; i < other.data_size_; i++)
    {
      push_back(other[i]);
    }
    data_size_ = other.data_size_;
  }
  /**
   * TODO Deconstructor
   */
  ~deque()
  {
    for (iterator i = start_; i != end_; i++)
    {
      (*i).~T();
    }
    for (int i = 0; i < len_; i++)
    {
      operator delete(ptrs_[i]);
    }
    operator delete(ptrs_);
  }
  /**
   * TODO assignment operator
   */
  deque &operator=(const deque &other)
  {
    if (this == &other)
    {
      return *this;
    }
    for (iterator i = start_; i != end_; i++)
    {
      (*i).~T();
    }
    end_ = start_;
    for (int i = 0; i < other.data_size_; i++)
    {
      push_back(other[i]);
    }
    data_size_ = other.data_size_;
    return *this;
  }
  /**
   * access specified element with bounds checking
   * throw index_out_of_bound if out of bound.
   */
  T &at(const size_t &pos)
  {
    int size =
        end_.cur_ - start_.cur_ + (end_.node_ - start_.node_) * blockSize;
    if (pos >= size)
    {
      throw index_out_of_bound();
    }
    int total_offset = pos + start_.cur_;
    int block_offset = total_offset / blockSize;
    int tar_pos = total_offset % blockSize;
    return (*(start_.node_ + block_offset))[tar_pos];
  }
  const T &at(const size_t &pos) const
  {
    int size =
        end_.cur_ - start_.cur_ + (end_.node_ - start_.node_) * blockSize;
    if (pos >= size)
    {
      throw index_out_of_bound();
    }
    int total_offset = pos + start_.cur_;
    int block_offset = total_offset / blockSize;
    int tar_pos = total_offset % blockSize;
    return (*(start_.node_ + block_offset))[tar_pos];
  }
  T &operator[](const size_t &pos)
  {
    int size =
        end_.cur_ - start_.cur_ + (end_.node_ - start_.node_) * blockSize;
    if (pos >= size)
    {
      throw index_out_of_bound();
    }
    int total_offset = pos + start_.cur_;
    int block_offset = total_offset / blockSize;
    int tar_pos = total_offset % blockSize;
    return (*(start_.node_ + block_offset))[tar_pos];
  }
  const T &operator[](const size_t &pos) const
  {
    int size =
        end_.cur_ - start_.cur_ + (end_.node_ - start_.node_) * blockSize;
    if (pos >= size)
    {
      throw index_out_of_bound();
    }
    int total_offset = pos + start_.cur_;
    int block_offset = total_offset / blockSize;
    int tar_pos = total_offset % blockSize;
    return (*(start_.node_ + block_offset))[tar_pos];
  }
  /**
   * access the first_ element
   * throw deq_is_empty when the deq is empty.
   */
  const T &front() const
  {
    if (empty())
    {
      throw container_is_empty();
    }
    return *start_;
  }
  /**
   * access the last_ element
   * throw deq_is_empty when the deq is empty.
   */
  const T &back() const
  {
    if (empty())
    {
      throw container_is_empty();
    }
    return *(end_ - 1);
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin()
  {
    return start_;
  }
  const_iterator cbegin() const
  {
    return start_;
  }
  /**
   * returns an iterator to the end.
   */
  iterator end()
  {
    return end_;
  }
  const_iterator cend() const
  {
    return end_;
  }
  /**
   * checks whether the deq is empty.
   */
  bool empty() const
  {
    return (!data_size_);
  }
  /**
   * returns the number of elements
   */
  size_t size() const
  {
    // return (end_ - start_);
    return data_size_;
  }
  /**
   * clears the contents
   */
  void clear()
  {
    for (iterator i = start_; i != end_; i++)
    {
      (*i).~T();
    }
    end_ = start_;
    data_size_ = 0;
  }
  /**
   * inserts elements at the specified locat on in the deq.
   * inserts value before pos
   * returns an iterator pointing to the inserted value
   *     throw if the iterator is invalid or it point to a wrong place.
   */
  iterator insert(iterator pos, const T &value)
  {
    int new_pos = pos - start_;
    if (new_pos < 0 || new_pos > data_size_)
    {
      throw index_out_of_bound();
    }
    if (data_size_ == 0)
    {
      end_++;
      new (*(pos.node_) + pos.cur_) T(value);
    }
    else if (new_pos <= data_size_ / 2)
    {
      if (start_.cur_ != 0)
      {
        new (*(start_.node_) + start_.cur_ - 1) T(*start_);
      }
      else
      {
        new (*(start_.node_ - 1) + blockSize - 1) T(*start_);
      }
      if (new_pos != 0)
      {
        for (iterator i = start_; i != pos - 1; i++)
        {
          (*i) = (*(i + 1));
        }
      }
      start_--;
      *(pos - 1) = value;
      if (CheckStart())
      {
        DoubleFrontSpace();
      }
    }
    else
    {
      if (pos != end_)
      {
        new (*(end_.node_) + end_.cur_) T(*(end_ - 1));
        for (iterator i = end_ - 1; i != pos; i--)
        {
          (*i) = (*(i - 1));
        }
        end_++;
        *pos = value;
      }
      else
      {
        new (*(end_.node_) + end_.cur_) T(value);
        end_++;
      }

      if (CheckEnd())
      {
        DoubleBackSpace();
      }
    }
    data_size_++;
    return start_ + new_pos;
  }
  /**
   * removes specified element at pos.
   * removes the element at pos.
   * returns an iterator pointing to the following element, if pos pointing to
   * the last_ element, end() will be returned. throw if the deq is empty,
   * the iterator is invalid or it points to a wrong place.
   */
  iterator erase(iterator pos)
  {
    int new_pos = pos - start_;
    if (empty())
    {
      throw container_is_empty();
    }
    if (new_pos < 0 || new_pos >= data_size_)
    {
      throw index_out_of_bound();
    }
    if (new_pos <= data_size_ / 2)
    {
      for (iterator i = pos; i != start_; i--)
      {
        (*i) = (*(i - 1));
      }
      (*start_).~T();
      start_++;
    }
    else
    {
      for (iterator i = pos; i != end_ - 1; i++)
      {
        (*i) = (*(i + 1));
      }
      (*(end_ - 1)).~T();
      end_--;
    }
    data_size_--;
    return start_ + new_pos;
  }
  /**
   * adds an element to the end
   */
  void push_back(const T &value)
  {
    new (*(end_.node_) + end_.cur_) T(value);
    end_++;
    if (CheckEnd())
    {
      DoubleBackSpace();
    }
    data_size_++;
  }
  /**
   * removes the last_ element
   *     throw when the deq is empty.
   */
  void pop_back()
  {
    if (empty())
    {
      throw container_is_empty();
    }
    (*(end_ - 1)).~T();
    end_--;
    data_size_--;
  }
  /**
   * inserts an element to the beginning.
   */
  void push_front(const T &value)
  {
    start_--;
    new (*(start_.node_) + start_.cur_) T(value);
    if (CheckStart())
    {
      DoubleFrontSpace();
    }
    data_size_++;
  }
  /**
   * removes the first_ element.
   *     throw when the deq is empty.
   */
  void pop_front()
  {
    if (empty())
    {
      throw container_is_empty();
    }
    (*start_).~T();
    start_++;
    data_size_--;
  }

 private:
  void init()
  {
    data_size_ = 0;
    len_ = 8;
    ptrs_ = static_cast<T **>(operator new(len_ * sizeof(T *)));
    for (int i = 0; i < len_; i++)
    {
      T *ptr = static_cast<T *>(operator new(blockSize * sizeof(T)));
      ptrs_[i] = ptr;
    }
    start_ = iterator((blockSize - 1) / 2, ptrs_ + (len_ - 1) / 2, this);
    // end_ = iterator((blockSize - 1) / 2, ptrs_ + (len_ - 1) / 2, this);
    end_ = start_;
  }
  void DoubleFrontSpace()
  {
    int start_block = start_.node_ - ptrs_;
    int end_block = end_.node_ - ptrs_;
    T **new_ptrs_ = static_cast<T **>(operator new(2 * len_ * sizeof(T *)));
    for (int i = len_; i < 2 * len_; i++)
    {
      new (new_ptrs_ + i) T *(*(ptrs_ + i - len_));
    }
    for (int i = 0; i < len_; i++)
    {
      T *tmp = static_cast<T *>(operator new(blockSize * sizeof(T)));
      new (new_ptrs_ + i) T *(tmp);
    }
    operator delete(ptrs_);
    start_.node_ = new_ptrs_ + len_ + start_block;
    end_.node_ = new_ptrs_ + len_ + end_block;
    ptrs_ = new_ptrs_;
    len_ = len_ * 2;
  }
  void DoubleBackSpace()
  {
    int start_block = start_.node_ - ptrs_;
    int end_block = end_.node_ - ptrs_;
    T **new_ptrs_ = static_cast<T **>(operator new(2 * len_ * sizeof(T *)));
    for (int i = 0; i <= len_ - 1; i++)
    {
      new (new_ptrs_ + i) T *(*(ptrs_ + i));
    }
    for (int i = len_; i <= 2 * len_ - 1; i++)
    {
      T *tmp = static_cast<T *>(operator new(blockSize * sizeof(T)));
      new (new_ptrs_ + i) T *(tmp);
    }
    operator delete(ptrs_);
    start_.node_ = new_ptrs_ + start_block;
    end_.node_ = new_ptrs_ + end_block;
    ptrs_ = new_ptrs_;
    len_ = len_ * 2;
  }
  inline bool CheckStart()
  {
    if (start_.cur_ == 0 && start_.node_ == ptrs_)
    {
      return true;
    }
    return false;
  }
  inline bool CheckEnd()
  {
    if (end_.cur_ == 15 && end_.node_ == ptrs_ + len_ - 1)
    {
      return true;
    }
    return false;
  }
  T **ptrs_;
  iterator start_;
  iterator end_;  // it will never has a value
  int len_;       // total len of the ptrs
  int data_size_;
};

}  // namespace sjtu

#endif