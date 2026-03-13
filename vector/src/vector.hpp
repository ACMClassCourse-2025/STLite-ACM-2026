#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
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
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
	    T* p = nullptr;
	    const vector* v = nullptr;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator(T* pointer, const vector* vector) : p(pointer), v(vector) {}
		iterator operator+(const int &n) const
		{
			return iterator(p + n, v);
			//TODO
		}
		iterator operator-(const int &n) const
		{
			return iterator(p - n, v);
			//TODO
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (v != rhs.v) {
				throw invalid_iterator();
			}
			return p - rhs.p;
			//TODO
		}
		iterator& operator+=(const int &n)
		{
			p += n;
			return *this;
			//TODO
		}
		iterator& operator-=(const int &n)
		{
			p -= n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			++p;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++p;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = *this;
			--p;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--p;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
			return *p;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if (p == rhs.p && v == rhs.v) {
				return true;
			}
			return false;
		}
		bool operator==(const const_iterator &rhs) const {
			if (p == rhs.p && v == rhs.v) {
				return true;
			}
			return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			if (p == rhs.p && v == rhs.v) {
				return false;
			}
			return true;
		}
		bool operator!=(const const_iterator &rhs) const {
			if (p == rhs.p && v == rhs.v) {
				return false;
			}
			return true;
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
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
	    const T* p;
	    const vector* v;
		/*TODO*/

	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator(T* pointer, const vector* vector) : p(pointer), v(vector) {}
		const_iterator(const iterator& it) : p(it.p), v(it.v) {}
		const_iterator operator+(const int &n) const
		{
		    return const_iterator(p + n, v);
			//TODO
		}
		const_iterator operator-(const int &n) const
		{
		    return const_iterator(p - n, v);
			//TODO
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
		    if (v != rhs.v) {
		        throw invalid_iterator();
		    }
		    return p - rhs.p;
			//TODO
		}
		const_iterator& operator+=(const int &n)
		{
		    p += n;
		    return *this;
			//TODO
		}
		const_iterator& operator-=(const int &n)
		{
		    p -= n;
		    return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
		    const_iterator tmp = *this;
		    ++p;
		    return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
		    ++p;
		    return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
		    const_iterator tmp = *this;
		    --p;
		    return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
		    --p;
		    return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const {
		    return *p;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
		    if (p == rhs.p && v == rhs.v) {
		        return true;
		    }
		    return false;
		}
		bool operator==(const const_iterator &rhs) const {
		    if (p == rhs.p && v == rhs.v) {
		        return true;
		    }
		    return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    if (p == rhs.p && v == rhs.v) {
		        return false;
		    }
		    return true;
		}
		bool operator!=(const const_iterator &rhs) const {
		    if (p == rhs.p && v == rhs.v) {
		        return false;
		    }
		    return true;
		}
	};
private:
        T* data;
        int curLen;
        int maxSize;
        // void doubleSpace() {
        //     T* tmp = data;
        //     maxSize *= 2;
        //     data = new T[maxSize];
        //     for (int i = 0; i < curLen; ++i) {
        //         data[i] = tmp[i];
        //     }
        //     delete[] tmp;
        // }
public:
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data(nullptr), curLen(0), maxSize(0) {}
	vector(const vector &other) : data(nullptr), curLen(0), maxSize(0) {
	    if (other.curLen > 0) {
	        // data = new T[other.curLen];
	        data = static_cast<T*>(::operator new(other.curLen * sizeof(T)));
	        curLen = other.curLen;
	        maxSize = other.curLen;
	        for (int i = 0; i < curLen; ++i) {
	            new (data + i) T(other.data[i]);
	            // data[i] = other.data[i];
	        }
	    }
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
	    for (size_t i = 0; i < curLen; ++i) {
	        data[i].~T();
	    }
	    ::operator delete(data);
	    // delete[] data;
	    // clear();
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
	    if (this == &other) {
	        return *this;
	    }
	    // if (data != nullptr) {
	    //     delete[] data;
	    // }
	    for (size_t i = 0; i < curLen; ++i) {
	        data[i].~T();
	    }
	    ::operator delete(data);
	    data = nullptr;
	    if (other.curLen > 0) {
	        data = static_cast<T*>(::operator new(other.curLen * sizeof(T)));
	        maxSize = other.curLen;
	        curLen = other.curLen;
	        for (size_t i = 0; i < curLen; ++i) {
	            new (data + i) T(other.data[i]);
	        }
	    }
	    else {
	        maxSize = 0;
	        curLen = 0;
	    }
	    return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
	    if (pos >= curLen) {
	        throw index_out_of_bound();
	    }
	    return data[pos];
	}
	const T & at(const size_t &pos) const {
	    if (pos >= curLen) {
	        throw index_out_of_bound();
	    }
	    return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
	    if (pos >= curLen) {
	        throw index_out_of_bound();
	    }
	    return data[pos];
	}
	const T & operator[](const size_t &pos) const {
	    if (pos >= curLen) {
	        throw index_out_of_bound();
	    }
	    return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
	    if (curLen == 0) {
	        throw container_is_empty();
	    }
	    return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
	    if (curLen == 0) {
	        throw container_is_empty();
	    }
	    return data[curLen - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    return iterator(data,this);
	}
	const_iterator begin() const {
	    return const_iterator(data,this);
	}
	const_iterator cbegin() const {
	    return const_iterator(data,this);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
	    return iterator(data + curLen,this);
	}
	const_iterator end() const {
	    return const_iterator(data + curLen,this);
	}
	const_iterator cend() const {
	    return const_iterator(data + curLen,this);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
	    if (curLen == 0) {
	        return true;
	    }
	    return false;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return curLen;
	}
	/**
	 * clears the contents
	 */
	void clear() {
	    // if (data != nullptr) {
	    //     delete[] data;
	    // }
	    for (int i = 0; i < curLen; ++i) {
	        data[i].~T();
	    }
	    curLen = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
	    int index = pos - begin();
	    if (curLen == maxSize) {
	    	int newSize = 0;
	        if (maxSize == 0) {
	            newSize = 1;
	        }
	    	else {
	    	    newSize = maxSize * 2;
	    	}
	        T *new_data = static_cast<T*>(::operator new(newSize * sizeof(T)));
	    	// T* new_data = new T[newSize];
	    	for (int i = 0; i < index; ++i) {
	    	    new (new_data + i) T(data[i]);
	    	    // new_data[i] = data[i];
	    	}
	        new (new_data + index) T(value);
	    	// new_data[index] = value;
	    	for (int i = index; i < curLen; ++i) {
	    	    new (new_data + i + 1) T(data[i]);
	    	    // new_data[i + 1] = data[i];
	    	}
	        for (int i = 0; i < curLen; ++i) {
	            data[i].~T();
	        }
	        ::operator delete(data);
	        // delete[] data;
	    	data = new_data;
	    	maxSize = newSize;
	    }
	    else {
	        for (int i = curLen; i > index; --i) {
	            new (data + i) T(data[i - 1]);
	            data[i - 1].~T();
	            // data[i] = data[i - 1];
	        }
	        new (data + index) T(value);
	    	// data[index] = value;
	    }
	    ++curLen;
	    return iterator(data + index, this);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
	    if (ind > curLen) {
	        throw index_out_of_bound();
	    }
	    return insert(begin() + ind,value);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
	    int index = pos - begin();
	    if (index >= curLen) {
	        throw invalid_iterator();
	    }
	    data[index].~T();
	    for (int i = index + 1; i < curLen; i++) {
	        new (data + i - 1) T(data[i]);
	        data[i].~T();
	        // data[i] = data[i + 1];
	    }
	    --curLen;
	    return iterator(data + index,this);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
	    if (ind >= curLen) {
	        throw index_out_of_bound();
	    }
	    return erase(begin() + ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
	    insert(end(),value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
	    if (curLen == 0) {
	        throw container_is_empty();
	    }
	    erase(end() - 1);
	}
};


}

#endif