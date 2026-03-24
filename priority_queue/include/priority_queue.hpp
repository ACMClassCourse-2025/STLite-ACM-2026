#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cmath>       // maybe you need it
#include <cstddef>     // for size_t
#include <functional>  // for std::less

#include "exceptions.hpp"

namespace sjtu
{

template <class T, class Compare = std::less<T>>
class priority_queue
{
 public:
  struct Node
  {
    Node* left_child_;
    Node* right_child_;
    T value_;
    Node(Node* left_child, Node* right_child, T value)
        : left_child_(left_child), right_child_(right_child), value_(value)
    {
    }
  };
  priority_queue()
  {
    size_ = 0;
    top_ = nullptr;
  }
  priority_queue(Node* top, size_t size)
  {
    top_ = top;
    size_ = size;
  }
  priority_queue(const priority_queue& other)
  {
    size_ = other.size_;
    top_ = clone(other.top_);
  }
  ~priority_queue()
  {
    deletePile(top_);
  }

  priority_queue& operator=(const priority_queue& other)
  {
    if (this == &other)
    {
      return *this;
    }
    clear();
    size_ = other.size_;
    top_ = clone(other.top_);
    return *this;
  }

  /**
   * @brief Push one element into the queue.
   * @note Its time complexity shall be O(log n).
   */
  void push(const T& e)
  {
    Node* new_node = new Node(nullptr, nullptr, e);
    priority_queue new_queue(new_node, 1);
    merge(new_queue);
  }

  /**
   * @return A const reference of the top element in the queue.
   * @throws container_is_empty when the top element does not exist.
   * @note Its time complexity shall be O(1).
   */
  const T& top() const
  {
    if (size_ == 0)
    {
      throw container_is_empty();
    }
    return top_->value_;
  }

  /**
   * @brief remove the top element in the queue.
   * @throws container_is_empty when the top element does not exist.
   * @note Its time complexity shall be O(n).
   */
  void pop()
  {
    if (size_ == 0)
    {
      throw container_is_empty();
    }
    Node* tmp = top_;
    top_ = mergePiles(top_->left_child_, top_->right_child_);
    delete tmp;
    size_--;
  }

  /**
   * @return number of elements in the queue.
   */
  size_t size() const
  {
    return size_;
  }

  /**
   * @return whether there is any element in the queue.
   */
  bool empty() const
  {
    return (size_ == 0);
  }

  /**
   * @brief Clear all elements in the queue.
   * @note Its time complexity shall be O(n).
   */
  void clear()
  {
    size_ = 0;
    deletePile(top_);
    top_ = nullptr;
  }

  /**
   * @brief Merge element sets of two queues.
   * @note Its time complexity shall be O(log n).
   */
  void merge(priority_queue& other)
  {
    if(this == &other)
    {
      return;
    }
    top_ = mergePiles(top_, other.top_);
    other.top_ = nullptr;
    size_ = size_ + other.size_;
    other.size_ = 0;
  }

 private:
  Node* mergePiles(Node* node1, Node* node2)
  {
    if (node1 == nullptr)
    {
      return node2;
    }
    if (node2 == nullptr)
    {
      return node1;
    }
    if (comp_(node1->value_, node2->value_))
    {
      node2->right_child_ = mergePiles(node1, node2->right_child_);
      Node* tmp = node2->left_child_;
      node2->left_child_ = node2->right_child_;
      node2->right_child_ = tmp;
      return node2;
    }
    else
    {
      node1->right_child_ = mergePiles(node1->right_child_, node2);
      Node* tmp = node1->left_child_;
      node1->left_child_ = node1->right_child_;
      node1->right_child_ = tmp;
      return node1;
    }
  }
  Node* clone(Node* node)
  {
    if (node == nullptr)
    {
      return nullptr;
    }
    Node* new_node = new Node(nullptr, nullptr, node->value_);
    new_node->left_child_ = clone(node->left_child_);
    new_node->right_child_ = clone(node->right_child_);
    return new_node;
  }
  void deletePile(Node* node)
  {
    if (node == nullptr)
    {
      return;
    }
    Node* left = node->left_child_;
    Node* right = node->right_child_;
    delete node;
    deletePile(left);
    deletePile(right);
  }
  Node* top_;
  size_t size_;
  Compare comp_;
};

}  // namespace sjtu

#endif