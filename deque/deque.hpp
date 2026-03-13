#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {
	template<class T>
	class deque {
	private:
		static constexpr int CHUNK_SIZE = 16;
		T** queue;
		int cur_size;
		int first_chunk;
		int first_index;
		int last_chunk;
		int last_index;
		int cnt;

		void grow_map(int need_front, int need_back) {
			int new_size = cur_size * 2;
			while (new_size < cnt / CHUNK_SIZE + need_front + need_back + 2) {
				new_size *= 2;
			}
			T** new_queue = new T*[new_size]();
			int new_first = (new_size - (last_chunk - first_chunk + 1)) / 2;
			for (size_t i = first_chunk; i <= last_chunk; ++i) {
				new_queue[new_first + (i - first_chunk)] = queue[i];
			}
			delete[] queue;
			queue = new_queue;
			last_chunk = new_first + (last_chunk - first_chunk);
			first_chunk = new_first;
			cur_size = new_size;
		}

	public:
		class const_iterator;
		class iterator {
		private:
			int chunk_index = 0;
			int inner_index = 0;
			const deque* deque = nullptr;
			/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		public:
			iterator(int chunk_idx, int inner_idx, const deque* deque) : chunk_index(chunk_idx), inner_index(inner_idx), deque(deque) {}
			/**
			 * return a new iterator which pointer n-next elements
			 *   even if there are not enough elements, the behaviour is **undefined**.
			 * as well as operator-
			 */
			iterator operator+(const int &n) const {
				if (n == 0) {
					return *this;
				}
				int index = (chunk_index - deque->first_chunk) * CHUNK_SIZE + (inner_index - deque->first_index);
				index += n;
				int new_chunk_index = (index + deque->first_index) / CHUNK_SIZE + deque->first_chunk;
				int new_inner_index = (index + deque->first_index) % CHUNK_SIZE;
				return iterator(new_chunk_index,new_inner_index,deque);

				//TODO
			}
			iterator operator-(const int &n) const {
				if (n == 0) {
					return *this;
				}
				int index = (chunk_index - deque->first_chunk) * CHUNK_SIZE + (inner_index - deque->first_index);
				index -= n;
				int new_chunk_index = (index + deque->first_index) / CHUNK_SIZE + deque->first_chunk;
				int new_inner_index = (index + deque->first_index) % CHUNK_SIZE;
				return iterator(new_chunk_index,new_inner_index,deque);

				//TODO
			}
			// return th distance between two iterator,
			// if these two iterators points to different vectors, throw invaild_iterator.
			int operator-(const iterator &rhs) const {
				if (deque != rhs.deque) {
					throw invalid_iterator();
				}
				int index1 = (chunk_index - deque->first_chunk) * CHUNK_SIZE + (inner_index - deque->first_index);
				int index2 = (rhs.chunk_index - rhs.deque->first_chunk) * CHUNK_SIZE + (rhs.inner_index - rhs.deque->first_index);
				return index1 - index2;
				//TODO
			}
			iterator operator+=(const int &n) {
				*this += n;
				return *this;
				//TODO
			}
			iterator operator-=(const int &n) {
				*this -= n;
				return *this;
				//TODO
			}
			/**
			 * TODO iter++
			 */
			iterator operator++(int) {
				iterator tmp = *this;
				++*this;
				return tmp;
			}
			/**
			 * TODO ++iter
			 */
			iterator& operator++() {
				if (inner_index + 1 < CHUNK_SIZE) {
					inner_index += 1;
				}
				else {
					chunk_index += 1;
					inner_index = 0;
				}
				return *this;
			}
			/**
			 * TODO iter--
			 */
			iterator operator--(int) {
				iterator tmp = *this;
				--*this;
				return tmp;
			}
			/**
			 * TODO --iter
			 */
			iterator& operator--() {
				if (inner_index != 0) {
					inner_index -= 1;
				}
				else {
					chunk_index -= 1;
					inner_index = CHUNK_SIZE - 1;
				}
				return *this;
			}
			/**
			 * TODO *it
			 */
			T& operator*() const {
				return deque->queue[chunk_index][inner_index];
			}
			/**
			 * TODO it->field
			 */
			T* operator->() const noexcept {
				return &(deque->queue[chunk_index][inner_index]);
			}
			/**
			 * a operator to check whether two iterators are same (pointing to the same memory).
			 */
			bool operator==(const iterator &rhs) const {
				if (chunk_index == rhs.chunk_index && inner_index == rhs.inner_index && deque == rhs.deque) {
					return true;
				}
				return false;
			}
			bool operator==(const const_iterator &rhs) const {
				if (chunk_index == rhs.chunk_index && inner_index == rhs.inner_index && deque == rhs.deque) {
					return true;
				}
				return false;
			}
			/**
			 * some other operator for iterator.
			 */
			bool operator!=(const iterator &rhs) const {
				if (*this == rhs) {
					return false;
				}
				return true;
			}
			bool operator!=(const const_iterator &rhs) const {
				if (*this == rhs) {
					return false;
				}
				return true;
			}
			friend class const_iterator;
			friend class deque;
		};
		class const_iterator {
			// it should has similar member method as iterator.
			//  and it should be able to construct from an iterator.
		private:
			int chunk_index;
			int inner_index;
			const deque* deque;
			// data members.
		public:
			const_iterator() : chunk_index(0),inner_index(0),deque(nullptr) {};
			const_iterator(int chunk_idx, int inner_idx, const deque* deque) : chunk_index(chunk_idx), inner_index(inner_idx), deque(deque) {
				// TODO
			};
			const_iterator(const iterator &other) : chunk_index(other.chunk_index), inner_index(other.inner_index), deque(other.deque) {
				// TODO
			}
			const_iterator operator+(const int &n) const {
				if (n == 0) {
					return *this;
				}
				int index = (chunk_index - deque->first_chunk) * CHUNK_SIZE + (inner_index - deque->first_index);
				index += n;
				int new_chunk_index = (index + deque->first_index) / CHUNK_SIZE + deque->first_chunk;
				int new_inner_index = (index + deque->first_index) % CHUNK_SIZE;
				return const_iterator(new_chunk_index,new_inner_index,deque);
				//TODO
			}
			const_iterator operator-(const int &n) const {
				if (n == 0) {
					return *this;
				}
				int index = (chunk_index - deque->first_chunk) * CHUNK_SIZE + (inner_index - deque->first_index);
				index -= n;
				int new_chunk_index = (index + deque->first_index) / CHUNK_SIZE + deque->first_chunk;
				int new_inner_index = (index + deque->first_index) % CHUNK_SIZE;
				return const_iterator(new_chunk_index,new_inner_index,deque);
				//TODO
			}
			int operator-(const const_iterator &rhs) const {
				if (deque != rhs.deque) {
					throw invalid_iterator();
				}
				int index1 = (chunk_index - deque->first_chunk) * CHUNK_SIZE + (inner_index - deque->first_index);
				int index2 = (rhs.chunk_index - rhs.deque->first_chunk) * CHUNK_SIZE + (rhs.inner_index - rhs.deque->first_index);
				return index1 - index2;
				//TODO
			}
			const_iterator operator+=(const int &n) {
				*this += n;
				return *this;
				//TODO
			}
			const_iterator operator-=(const int &n) {
				*this -= n;
				return *this;
				//TODO
			}
			const_iterator operator++(int) {
				const_iterator tmp = *this;
				++*this;
				return tmp;
			}
			const_iterator& operator++() {
				if (inner_index + 1 < CHUNK_SIZE) {
					inner_index += 1;
				}
				else {
					chunk_index += 1;
					inner_index = 0;
				}
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator tmp = *this;
				--*this;
				return tmp;
			}
			const_iterator& operator--() {
				if (inner_index != 0) {
					inner_index -= 1;
				}
				else {
					chunk_index -= 1;
					inner_index = CHUNK_SIZE - 1;
				}
				return *this;
			}
			const T& operator*() const {
				return deque->queue[chunk_index][inner_index];
			}
			const T* operator->() const noexcept {
				return &(deque->queue[chunk_index][inner_index]);
			}
			bool operator==(const iterator &rhs) const {
				if (chunk_index == rhs.chunk_index && inner_index == rhs.inner_index && deque == rhs.deque) {
					return true;
				}
				return false;
			}
			bool operator==(const const_iterator &rhs) const {
				if (chunk_index == rhs.chunk_index && inner_index == rhs.inner_index && deque == rhs.deque) {
					return true;
				}
				return false;
			}
			bool operator!=(const iterator &rhs) const {
				if (*this == rhs) {
					return false;
				}
				return true;
			}
			bool operator!=(const const_iterator &rhs) const {
				if (*this == rhs) {
					return false;
				}
				return true;
			}
			friend class deque;
		};
		/**
		 * TODO Constructors
		 */
		deque() {
			cur_size = 8;
			queue = new T* [cur_size]();
			first_chunk = cur_size / 2;
			first_index = 0;
			last_chunk = cur_size / 2;
			last_index = 0;
			cnt = 0;

		}
		deque(const deque &other) {
			cur_size = other.cur_size;
			queue = new T* [cur_size]();
			first_chunk = other.first_chunk;
			first_index = other.first_index;
			last_chunk = other.last_chunk;
			last_index = other.last_index;
			cnt = other.cnt;
			for (int i = first_chunk; i <= last_chunk; ++i) {
				if (other.queue[i] != nullptr) {
					T* new_chunk = static_cast<T*> (::operator new(CHUNK_SIZE * sizeof(T)));
					int start = 0 ,end = CHUNK_SIZE;
					if (i == first_chunk) {
						start = first_index;
					}
					if (i == last_chunk) {
						end = last_index + 1;
					}
					for (int j = start; j < end; ++j) {
						new (new_chunk + j) T(other.queue[i][j]);
					}
					queue[i] = new_chunk;
				}
			}
		}
		/**
		 * TODO Deconstructor
		 */
		~deque() {
			clear();
			delete[] queue;
		}
		/**
		 * TODO assignment operator
		 */
		deque &operator=(const deque &other) {
			first_chunk = other.first_chunk;
			first_index = other.first_index;
			last_chunk = other.last_chunk;
			last_index = other.last_index;
			cnt = other.cnt;
			cur_size = other.cur_size;
			for (int i = first_chunk; i <= last_chunk; ++i) {
				if (other.queue[i] != nullptr) {
					T* new_chunk = static_cast<T*> (::operator new(CHUNK_SIZE * sizeof(T)));
					int start = 0, end = CHUNK_SIZE;
					if (i == first_chunk) {
						start = first_index;
					}
					else if (i == last_chunk) {
						end = last_index + 1;
					}
					for (int j = start; j < end; ++j) {
						new (new_chunk + j) T (other.queue[i][j]);
					}
					queue[i] = new_chunk;
				}
			}
			return *this;
		}
		/**
		 * access specified element with bounds checking
		 * throw index_out_of_bound if out of bound.
		 */
		T & at(const size_t &pos) {
			if (pos > cnt) {
				throw index_out_of_bound();
			}
			int index = pos + first_index;
			int chunk = first_chunk + index / CHUNK_SIZE;
			int inner_index = index % CHUNK_SIZE;
			return queue[chunk][inner_index];
		}
		const T & at(const size_t &pos) const {
			if (pos > cnt) {
				throw index_out_of_bound();
			}
			int index = pos + first_index;
			int chunk = first_chunk + index / CHUNK_SIZE;
			int inner_index = index % CHUNK_SIZE;
			return queue[chunk][inner_index];
		}
		T & operator[](const size_t &pos) {
			return at(pos);
		}
		const T & operator[](const size_t &pos) const {
			return at(pos);
		}
		/**
		 * access the first element
		 * throw container_is_empty when the container is empty.
		 */
		const T & front() const {
			if (cnt == 0) {
				throw container_is_empty();
			}
			return queue[first_chunk][first_index];
		}
		/**
		 * access the last element
		 * throw container_is_empty when the container is empty.
		 */
		const T & back() const {
			if (cnt == 0) {
				throw container_is_empty();
			}
			return queue[last_chunk][last_index];
		}
		/**
		 * returns an iterator to the beginning.
		 */
		iterator begin() {
			return iterator(first_chunk,first_index,*this);
		}
		const_iterator cbegin() const {
			return const_iterator(first_chunk,first_index,*this);
		}
		/**
		 * returns an iterator to the end.
		 */
		iterator end() {
			return iterator(last_chunk,last_index,*this);
		}
		const_iterator cend() const {
			return const_iterator(last_chunk,last_index,*this);
		}
		/**
		 * checks whether the container is empty.
		 */
		bool empty() const {
			if (cnt == 0) {
				return true;
			}
			return false;
		}
		/**
		 * returns the number of elements
		 */
		size_t size() const {
			return cnt;
		}
		/**
		 * clears the contents
		 */
		void clear() {
			for (int i = first_chunk; i <= last_chunk; ++i) {
				if (queue[i] != nullptr) {
					int start = 0, end = CHUNK_SIZE;
					if (i == first_chunk) {
						start = first_index;
					}
					if (i == last_chunk) {
						end = last_index + 1;
					}
					for (int j = start; j < end; ++j) {
						queue[i][j].~T();
					}
					::operator delete(queue[i]);
					queue[i] = nullptr;
				}
			}
			first_chunk = last_chunk = cur_size / 2;
			first_index = last_index = 0;
			cnt = 0;
		}
		/**
		 * inserts elements at the specified locat on in the container.
		 * inserts value before pos
		 * returns an iterator pointing to the inserted value
		 *     throw if the iterator is invalid or it point to a wrong place.
		 */
		iterator insert(iterator pos, const T &value) {
			if (pos.deque != this) {
				throw invalid_iterator();
			}
			if (pos == begin()) {
				push_front(value);
				return begin();
			}
			if (pos == end()) {
				push_back(value);
				return --end();
			}
			iterator it = end();
			while (it != pos) {
				iterator pre = it - 1;
				*it = *pre;
				--it;
			}
			*pos = value;
			++cnt;
			if (last_index + 1 == CHUNK_SIZE) {
				if (last_chunk + 1 >= cur_size) {
					grow_map(0, 1);
				}
				T* new_block = static_cast<T*>(::operator new(CHUNK_SIZE * sizeof(T)));
				queue[++last_chunk] = new_block;
				last_index = 0;
			}
			else {
				++last_index;
			}
			return pos;
		}
		/**
		 * removes specified element at pos.
		 * removes the element at pos.
		 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
		 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
		 */
		iterator erase(iterator pos) {
			if (cnt == 0) {
				throw container_is_empty();
			}
			if (pos.deque != this) {
				throw invalid_iterator();
			}
			if (pos == end()) {
				throw invalid_iterator();
			}
			if (pos == begin()) {
				pop_front();
				return begin();
			}
			if (pos == --end()) {
				pop_back();
				return end();
			}
			iterator cur = pos;
			iterator next = pos + 1;
			while (next != end()) {
				*cur = *next;
				++cur;
				++next;
			}
			pop_back();
			return pos;
		}
		/**
		 * adds an element to the end
		 */
		void push_back(const T &value) {
			if (cnt == 0) {
				if (first_chunk >= cur_size) {
					grow_map(1,1 );
				}
				queue[first_chunk] = static_cast<T*>(::operator new(CHUNK_SIZE * sizeof(T)));
				new (queue[first_chunk][first_index]) T(value);
				last_chunk = first_chunk;
				last_index = first_index;
				cnt = 1;
				return;
			}
			if (last_index + 1 < CHUNK_SIZE) {
				new (queue[last_chunk][last_index + 1]) T(value);
				++last_index;
				++cnt;
			}
			else {
				if (last_chunk + 1 >= cur_size) {
					grow_map(0, 1);
				}
				T* new_chunk = static_cast<T*>(::operator new(CHUNK_SIZE * sizeof(T)));
				queue[++last_chunk] = new_chunk;
				new (new_chunk) T(value);
				last_index = 0;
				++cnt;
			}
		}
		/**
		 * removes the last element
		 *     throw when the container is empty.
		 */
		void pop_back() {
			if (cnt == 0) {
				throw container_is_empty();
			}
			queue[last_chunk][last_index].~T();
			if (last_index > 0) {
				--last_index;
			}
			else {
				::operator delete (queue[last_chunk]);
				queue[last_chunk] = nullptr;
				if (last_chunk > first_chunk) {
					--last_chunk;
					last_index = CHUNK_SIZE - 1;
				}
				else {
					first_chunk = last_chunk = cur_size / 2;
					first_index = last_index = 0;
				}
			}
			--cnt;
		}
		/**
		 * inserts an element to the beginning.
		 */
		void push_front(const T &value) {
			if (cnt == 0) {
				push_back(value);
				return;
			}
			if (first_index > 0) {
				new (queue[first_chunk][first_index - 1]) T(value);
				--first_index;
				++cnt;
			}
			else {
				if (first_chunk == 0) {
					grow_map(1, 0);
				}
				T* new_chunk = static_cast<T*>(::operator new(CHUNK_SIZE * sizeof(T)));
				queue[--first_index] = new_chunk;
				new (new_chunk + CHUNK_SIZE - 1) T(value);
				first_index = CHUNK_SIZE - 1;
				++cnt;
			}
		}
		/**
		 * removes the first element.
		 *     throw when the container is empty.
		 */
		void pop_front() {
			if (cnt == 0) {
				throw container_is_empty();
			}
			queue[first_chunk][first_index].~T();
			if (first_index + 1 < CHUNK_SIZE) {
				++first_index;
			}
			else {
				::operator delete (queue[first_chunk]);
				queue[first_chunk] = nullptr;
				if (first_chunk < last_chunk) {
					++first_chunk;
					first_index = 0;
				}
				else {
					first_chunk = last_chunk = cur_size / 2;
					first_index = last_index = 0;
				}
			}
			--cnt;
		}
	};
}

#endif
