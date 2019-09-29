#pragma once

#include "../fast_vector/fast_vector.h"


/**
 * Custom implementations
 */
namespace my {
	template <typename T>
	using comparison = bool (T &, T &);

	template <typename T>
	bool greater(T & first, T & second) {
		return first > second;
	}

	template <typename T>
	bool less(T & first, T & second) {
		return first < second;
	}

	/**
     * Custom heap implementation
     */
	template <
		typename T,
		typename Allocator = std::allocator<T>
	>
	class heap {
	public:
		 /**
         * Allows to access template type T.
         * Despite value_type is defined I prefer
         * using T.
         */
        using value_type = T;

        /**
         * Allows to access allocator type.
         * Despite allocator_type is defined I prefer
         * using Allocator.
         */
        using allocator_type = Allocator;

        /**
         * Simplifies access to allocator traits
         */
        using allocator_traits = std::allocator_traits<allocator_type>;

        /**
         * Generalizes memory menagement types
         */
        using       size_type = typename allocator_traits::size_type;
        using difference_type = typename allocator_traits::difference_type;

        /**
         * Generalizes memory menagement types
         */
        using       pointer = typename allocator_traits::pointer;
        using const_pointer = typename allocator_traits::const_pointer;

        /**
         * Generalizes memory menagement types
         */
        using       reference =       value_type &;
        using const_reference = const value_type &;

        /**
         * Generalizes iterator types
         */
        using               iterator =       pointer;
        using         const_iterator = const_pointer;
        using       reverse_iterator =       pointer;
        using const_reverse_iterator = const_pointer;

        static_assert(
            std::is_same<typename allocator_type::value_type, value_type>::value,
            "Allocator::value_type must be same type as value_type"
        );

        /**
         * Returns begin random_access_iterator
         */
        iterator begin() noexcept {
            return the_storage.begin();
        }

        /**
         * Returns end random_access_iterator
         */
        iterator end() noexcept {
            return the_storage.end();
        }

        /**
         * Returns begin const random_access_iterator
         */
        const_iterator cbegin() const noexcept {
            return the_storage.cbegin();
        }

        /**
         * Returns end const random_access_iterator
         */
        const_iterator cend() const noexcept {
            return the_storage.cend();
        }

        /**
         * Returns begin reverse random_access_iterator
         */
        reverse_iterator rbegin() noexcept {
            return the_storage.rbegin();
        }

        /**
         * Returns end reverse random_access_iterator
         */
        reverse_iterator rend() noexcept {
            return the_storage.rend();
        }

        /**
         * Returns begin const reverse random_access_iterator
         */
        const_reverse_iterator crbegin() const noexcept {
            return the_storage.crbegin();
        }

        /**
         * Returns end const reverse random_access_iterator
         */
        const_reverse_iterator crend() const noexcept {
            return the_storage.crend();
        }

        /**
         * Returns the count of elements
         */
        size_type size() const noexcept {
            return the_storage.size();
        }

        /**
         * Returns the size of the inner storage
         */
        size_type capacity() const noexcept {
            return the_storage.capacity();
        }

        /**
         * Returns the maximum possible count of elements
         */
        size_type max_size() const noexcept {
            return the_storage.max_size();
        }

        /**
         * Returns an instance of allocator
         */
        Allocator get_allocator() const noexcept {
            return the_storage.get_allocator();
        }

        /**
         * Returns true if size is 0
         */
        bool empty() const noexcept {
            return the_storage.empty();
        }

        /**
         * Returns a reference to the
         * element at the given position
         */
        reference operator [] (size_type n) {
            return the_storage[n];
        }

        /**
         * Returns a const_reference to the
         * element at the given position
         */
        const_reference operator [] (size_type n) const {
            return the_storage[n];
        }

        /**
         * Returns a reference to the
         * element at the given position.
         * Throws out_of_range on error
         */
        reference at(size_type n) {
            return the_storage.at(n);
        }

        /**
         * Returns a const_reference to the
         * element at the given position.
         * Throws out_of_range on error
         */
        const_reference at(size_type n) const {
            return the_storage.at(n);
        }

        /**
         * Returns a reference to the
         * first element
         */
        reference front() {
            return the_storage.front();
        }

        /**
         * Returns a const_reference to the
         * first element
         */
        const_reference front() const {
            return the_storage.front();
        }

        /**
         * Returns a reference to the
         * last element
         */
        reference back() {
            return the_storage.back();
        }

        /**
         * Returns a const_reference to the
         * last element
         */
        const_reference back() const {
            return the_storage.back();
        }

        /**
         * Returns a pointer to the
         * internal storage
         */
        pointer data() noexcept {
            return the_storage.data();
        }

        /**
         * Returns a const_pointer to the
         * internal storage
         */
        const_pointer data() const noexcept {
            return the_storage.data();
        }

        /**
         * Constructs an empty heap
         */
        explicit heap(
        	comparison<T> comparison = less<T>,
            const Allocator & allocator = Allocator()
        ) : the_comparison(comparison), the_storage(0, allocator) {}

        /**
         * Constructs a heap via copying
         * items between iterators
         */
        template <typename InputIterator>
        heap(
            InputIterator first,
            InputIterator last,
            comparison<T> comparison = less<T>,
            const Allocator & allocator = Allocator()
        ) : the_comparison(comparison), the_storage(first, last, allocator) {
        	invalidate();
        }

        /**
         * Removes the top element
         */
        void pop_top() {
        	the_storage.erase(the_storage.begin());
        	invalidate();
        }

	private:
		comparison<T> * the_comparison;
		fast_vector<T, Allocator> the_storage;

		/**
		 * Drows the given element down
		 * to it's proper place.
		 * Linear time complexity
		 */
		void drown(pointer it) {
			auto index = it - begin();

			auto RIGHT = it + index + 1;
			auto LEFT  = it + index;

			bool has_right = RIGHT < end();
			bool has_left  = LEFT  < end();

			bool must_swap_right = false;
			bool must_swap_left  = false;

			if (has_right) {
				must_swap_right = the_comparison(*it, *RIGHT);
			}

			if (has_left) {
				must_swap_left = the_comparison(*it, *LEFT);
			}

			if (must_swap_right && must_swap_left) {
				auto target = RIGHT;

				if (the_comparison(*RIGHT, *LEFT)) {
					target = LEFT;
				}

				std::swap(*it, *target);
				drown(target);
			} else if (must_swap_right) {
				std::swap(*it, *RIGHT);
				drown(RIGHT);
			} else if (must_swap_left) {
				std::swap(*it, *LEFT);
				drown(LEFT);
			}
		}

		/**
		 * Ensures that all elements
		 * are at the proper places
		 */
		void invalidate() {
			for (auto it = rbegin(); it != rend(); it--) {
				drown(it);
			}
		}
	};
}