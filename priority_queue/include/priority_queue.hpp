#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cmath>       // maybe you need it
#include <cstddef>     // for size_t
#include <functional>  // for std::less

#include "exceptions.hpp"

namespace sjtu {

template <class T, class Compare = std::less<T>>
class priority_queue {
public:
    priority_queue();
    priority_queue(const priority_queue&);
    ~priority_queue();

    priority_queue& operator=(const priority_queue&);

    /**
     * @brief Push one element into the queue.
     * @note Its time complexity shall be O(log n).
     */
    void push(const T&);

    /**
     * @return A const reference of the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(1).
     */
    const T& top() const;

    /**
     * @brief remove the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(n).
     */
    void pop();

    /**
     * @return number of elements in the queue.
     */
    size_t size() const;

    /**
     * @return whether there is any element in the queue.
     */
    bool empty() const;

    /**
     * @brief Clear all elements in the queue.
     * @note Its time complexity shall be O(n).
     */
    void clear();

    /**
     * @brief Merge element sets of two queues.
     * @note Its time complexity shall be O(log n).
     */
    void merge(priority_queue&);
};

}  // namespace sjtu

#endif