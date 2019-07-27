#pragma once

// for debug
// #include <iostream>

// for std::swap
#include <utility>
// for std::numeric_limits<size_type>::max
#include <limits>
// for std::allocator_traits
#include <memory>

// for std::memcpy
#include <cstring>
// for pow(2, ...)
#include <cmath>
// for std::is_same
#include <type_traits>

/**
 * Used by the default constructor
 * for allocating proper starting capacity
 */
#define VECTOR_DEFAULT_CAPACITY 10


/**
 * Custom implementations
 */
namespace my {
    /**
     * Custom vector-like container.
     * Built for self-education purposes
     */
    template <
        typename T,
        typename Allocator = std::allocator<T>
    >
    class vector {
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
            return the_begin;
        }

        /**
         * Returns end random_access_iterator
         */
        iterator end() noexcept {
            return the_end;
        }

        /**
         * Returns begin const random_access_iterator
         */
        const_iterator cbegin() const noexcept {
            return the_begin;
        }

        /**
         * Returns end const random_access_iterator
         */
        const_iterator cend() const noexcept {
            return the_end;
        }

        /**
         * Returns begin reverse random_access_iterator
         */
        reverse_iterator rbegin() noexcept {
            return the_end - 1;
        }

        /**
         * Returns end reverse random_access_iterator
         */
        reverse_iterator rend() noexcept {
            return the_begin - 1;
        }

        /**
         * Returns begin const reverse random_access_iterator
         */
        const_reverse_iterator crbegin() const noexcept {
            return the_end - 1;
        }

        /**
         * Returns end const reverse random_access_iterator
         */
        const_reverse_iterator crend() const noexcept {
            return the_begin - 1;
        }

        /**
         * Returns the count of elements
         */
        size_type size() const noexcept {
            return the_end - the_begin;
        }

        /**
         * Returns the size of the inner storage
         */
        size_type capacity() const noexcept {
            return the_capacity;
        }

        /**
         * Returns the maximum possible count of elements
         */
        size_type max_size() const noexcept {
            return std::numeric_limits<size_type>::max();
        }

        /**
         * Returns an instance of allocator
         */
        Allocator get_allocator() const noexcept {
            return the_allocator;
        }

        /**
         * Returns true if size is 0
         */
        bool empty() const noexcept {
            return the_begin == the_end;
        }

        /**
         * Returns a reference to the
         * element at the given position
         */
        reference operator [] (size_type n) {
            return the_begin[n];
        }

        /**
         * Returns a const_reference to the
         * element at the given position
         */
        const_reference operator [] (size_type n) const {
            return the_begin[n];
        }

        /**
         * Returns a reference to the
         * element at the given position.
         * Throws out_of_range on error
         */
        reference at(size_type n) {
            if (n >= size())
                throw std::out_of_range("Requested index is greater than size");
            return the_begin[n];
        }

        /**
         * Returns a const_reference to the
         * element at the given position.
         * Throws out_of_range on error
         */
        const_reference at(size_type n) const {
            if (n >= size())
                throw std::out_of_range("Requested index is greater than size");
            return the_begin[n];
        }

        /**
         * Returns a reference to the
         * first element
         */
        reference front() {
            return *the_begin;
        }

        /**
         * Returns a const_reference to the
         * first element
         */
        const_reference front() const {
            return *the_begin;
        }

        /**
         * Returns a reference to the
         * last element
         */
        reference back() {
            return *(the_end - 1);
        }

        /**
         * Returns a const_reference to the
         * last element
         */
        const_reference back() const {
            return *(the_end - 1);
        }

        /**
         * Returns a pointer to the
         * internal storage
         */
        pointer data() noexcept {
            return the_begin;
        }

        /**
         * Returns a const_pointer to the
         * internal storage
         */
        const_pointer data() const noexcept {
            return the_begin;
        }

        /**
         * Destructs every item and deallocates
         * space of the internal storage
         */
        ~vector() {
            for (auto it = the_begin; it != the_end; it++) {
                (*it).~T();
            }

            allocator_traits::deallocate(the_allocator, the_begin, the_capacity);
        }

        /**
         * Constructs a vector with the given
         * count of filler copies
         */
        vector(
            size_type size,
            const T & filler,
            const Allocator & allocator = Allocator()
        ) : the_allocator(allocator) {
            force_reserve(size);

            while (the_end != the_begin + size) {
                new(the_end) T(filler);
                the_end++;
            }
        }

        /**
         * Constructs a vector with the given
         * count of defaults
         */
        explicit vector(
            size_type size,
            const Allocator & allocator = Allocator()
        ) : vector(size, T(), allocator) {}

        /**
         * Constructs a vector with default capacity
         * and no elements
         */
        explicit vector(
            const Allocator & allocator = Allocator()
        ) : the_allocator(allocator) {
            force_reserve(VECTOR_DEFAULT_CAPACITY);
        }

        /**
         * Constructs a vector via copying
         * items between iterators
         */
        template <typename InputIterator>
        vector(
            InputIterator first,
            InputIterator last,
            const Allocator & allocator = Allocator()
        ) : the_allocator(allocator) {
            size_type size = std::distance(first, last);
            force_reserve(size);
            auto from = first;

            while (from != last) {
                new(the_end) T(*from);
                the_end++;
                from++;
            }
        }

        /**
         * Constructs a vector via copying
         * items from the given initialization list
         */
        vector(
            std::initializer_list<T> list,
            const Allocator & allocator = Allocator()
        ) : vector(list.begin(), list.end(), allocator) {}

        /**
         * Constructs a copy of the given vector.
         * The capacity of the copy will = it's size
         */
        vector(
            const vector & other,
            const Allocator & allocator
        ) : vector(other.cbegin(), other.cend(), allocator) {}

        /**
         * Constructs a copy of the given vector.
         * The capacity of the copy will = it's size
         */
        vector(
            const vector & other
        ) : vector(
            other.cbegin(),
            other.cend(),
            allocator_traits::select_on_container_copy_construction(other.the_allocator)
        ) {}

        /**
         * Moves contents of other into itself
         */
        vector(
            vector && other
        ) : the_allocator(other.the_allocator) {
            raw_swap(std::move(other));
        }

        /**
         * Moves contents of other into itself
         */
        vector(
            vector && other,
            const Allocator & allocator
        ) : the_allocator(allocator) {
            if (the_allocator == other.the_allocator) {
                raw_swap(std::move(other));
            } else {
                // we are told to used the specific allocator
                // so the elements must be located in it's space
                force_reserve(other.size());

                auto from = other.the_begin;

                while (from != other.the_end) {
                    new(the_end) T(std::move(*from));
                    the_end++;
                    from++;
                }
            }
        }

        /**
         * Swaps inner contents of the two vectors
         */
        void swap(vector && other) {
            if (the_allocator == other.the_allocator) {
                raw_swap(std::move(other));
            } else if (allocator_traits::propagate_on_container_swap::value == true) {
                raw_swap(std::move(other));
            } else {
                // according to the docs this branch is an undefined behaviour.
                blyat_swap(std::move(other));
            }
        }

        /**
         * Swaps inner contents of the two vectors
         */
        void swap(vector & other) {
            swap(std::move(other));
        }

        /**
         * Copies contents of another vector
         * and destroys previous
         */
        void operator = (const vector & other) {
            if (allocator_traits::propagate_on_container_copy_assignment::value == true) {
                vector copy(other, other.the_allocator);
                raw_swap(std::move(copy));
            } else {
                vector copy(other, the_allocator);
                raw_swap(std::move(copy));
            }
        }

        /**
         * Accuires contents of another vector
         * and destroys previous
         */
        void operator = (vector && other) {
            if (the_allocator == other.the_allocator) {
                raw_swap(std::move(other));
            } else if (allocator_traits::propagate_on_container_move_assignment::value == true) {
                raw_swap(std::move(other));
            } else {
                vector temp(std::move(other), the_allocator);
                raw_swap(std::move(temp));
            }
        }

        /**
         * Copies contents of initialization list
         * and destroys previous
         */
        void operator = (std::initializer_list<T> list) {
            vector copy(list, the_allocator);
            raw_swap(std::move(copy));
        }

        /**
         * Fills with the filler size times
         * and destroys the previous data
         */
        void assign(size_type size, const T & filler) {
            vector copy(size, filler, the_allocator);
            raw_swap(std::move(copy));
        }

        /**
         * Fills with the contents between the iterators
         * and destroys the previous data
         */
        template <typename InputIterator>
        void assign(InputIterator first, InputIterator last) {
            vector copy(first, last, the_allocator);
            raw_swap(std::move(copy));
        }

        /**
         * Removes everythnig
         */
        void clear() noexcept {
            vector empty(the_allocator);
            raw_swap(std::move(empty));
        }

        /**
         * Allocates much enough memory to
         * fit a certain count of elements
         */
        void reserve(size_type size) {
            if (size > the_capacity) {
                force_reserve(size);
            }
        }

        /**
         * Reduces the capacity so that
         * it equals the size.
         */
        void shrink_to_fit() {
            vector copy(*this, the_allocator);
            raw_swap(std::move(copy));
        }

        /**
         * Makes it contain the exact
         * count of elements. Fills with
         * filler if needed
         */
        void resize(size_type size, const T & filler) {
            reserve(size);

            while (the_end < the_begin + size) {
                new(the_end) T(filler);
                the_end++;
            }

            the_end = the_begin + size;
        }

        /**
         * Makes it contain the exact
         * count of elements. Fills with
         * defaults if needed
         */
        void resize(size_type size) {
            resize(size, T());
        }

        /**
         * Allocates the item directly in the
         * preallocated inner storage
         */
        template <typename... K>
        void emplace_back(K &&... arguments) {
            ensure_can_add_one();
            new(the_end) T(std::forward<K>(arguments)...);
            the_end++;
        }

        /**
         * Allocates the item directly in the
         * preallocated inner storage
         */
        template <typename... K>
        iterator emplace(const_iterator position, K &&... arguments) {
            auto place = const_cast<iterator>(position);
            // if reallocation happens
            // place will become an invalid pointer
            auto old_begin = the_begin;
            ensure_can_add_one();
            place = place - old_begin + the_begin;

            shift_right(place, 1);

            new(place) T(std::forward<K>(arguments)...);
            the_end++;

            return place;
        }

        /**
         * Adds element to the vector
         */
        void push_back(const T & item) {
            emplace_back(item);
        }

        /**
         * Inserts element into the given position
         */
        iterator insert(const_iterator position, const T & item) {
            return emplace(position, item);
        }

        /**
         * Inserts element into the given position
         */
        iterator insert(const_iterator position, T && item) {
            return emplace(position, std::move(item));
        }

        /**
         * Inserts element into the given position
         */
        iterator insert(
            const_iterator position,
            size_type size,
            const T & filler
        ) {
            auto place = const_cast<iterator>(position);
            // if reallocation happens
            // place will become an invalid pointer
            auto old_begin = the_begin;
            ensure_can_add(size);
            place = place - old_begin + the_begin;

            shift_right(place, size);

            for (auto it = place; it < place + size; it++) {
                new(it) T(filler);
            }

            the_end += size;
            return place;
        }

        /**
         * Inserts elements between first and last
         * into the position
         */
        template <typename InputIterator>
        iterator insert(
            const_iterator position,
            InputIterator first,
            InputIterator last
        ) {
            auto place = const_cast<iterator>(position);
            size_type size = std::distance(first, last);
            // if reallocation happens
            // place will become an invalid pointer
            auto old_begin = the_begin;
            ensure_can_add(size);
            place = place - old_begin + the_begin;

            shift_right(place, size);

            for (size_type it = 0; it < size; it++) {
                new(place + it) T(*(first + it));
            }

            the_end += size;
            return place;
        }

        /**
         * Copies elements from the list
         * into the position
         */
        iterator insert(
            const_iterator position,
            std::initializer_list<T> list
        ) {
            return insert(position, list.begin(), list.end());
        }

        /**
         * Destroys last element
         */
        void pop_back() {
            back().~T();
            the_end--;
        }

        /**
         * Removes one element at the position
         */
        iterator erase(const_iterator position) {
            auto place = const_cast<iterator>(position);

            shift_left(place, 1);
            the_end--;

            return place;
        }

        /**
         * Removes elements between first and last
         */
        iterator erase(const_iterator first, const_iterator last) {
            auto first_place = const_cast<iterator>(first);
            auto last_place  = const_cast<iterator>(last);

            size_type size = std::distance(first_place, last_place);
            shift_left(first_place, size);
            the_end -= size;

            return first_place;
        }

    private:
        pointer   the_end       = nullptr;
        pointer   the_begin     = nullptr;
        size_type the_capacity  = 0;
        Allocator the_allocator;

        /**
         * Reallocate the inner storage
         * to satisfy the new capacity
         */
        void force_reserve(size_type new_capacity) {
            size_type old_size = 0;

            // allocate new space
            pointer new_place = allocator_traits::allocate(the_allocator, new_capacity);

            // if we had smth previously
            if (the_begin != the_end) {
                old_size = the_end - the_begin;
                // copy contents
                std::memcpy(new_place, the_begin, sizeof(T) * old_size);
                // deallocate old space
                allocator_traits::deallocate(the_allocator, the_begin, the_capacity);
            }

            // apply changes
            the_begin = new_place;
            the_capacity = new_capacity;
            the_end = the_begin + old_size;
        }

        /**
         * Shifts all elements to the
         * right. Note that it does not affect size.
         * Position points to the first element from
         * the left that will be shifted and size is
         * the amount of steps it'll travel
         */
        void shift_right(iterator position, size_type size) {
            for (auto it = the_end; it > position; it--) {
                std::memcpy(it - 1 + size, it - 1, sizeof(T));
            }
        }

        /**
         * Shifts all elements to the
         * left. Note that it does not affect size.
         * Position points to that place where the first
         * element from the left will appear after
         * shifting.
         */
        void shift_left(iterator position, size_type size) {
            for (auto it = position; it < the_end; it++) {
                std::memcpy(it, it + size, sizeof(T));
            }
        }

        /**
         * Extends the internal storage to
         * contain at least one more element.
         */
        void ensure_can_add_one() {
            auto current_size = size();

            if (current_size >= the_capacity) {
                auto max = max_size();

                if (current_size == max)
                    throw std::length_error("Maximum size reached");

                if (the_capacity > max / 2) {
                    force_reserve(max);
                } else {
                    force_reserve(the_capacity * 2);
                }
            }
        }

        /**
         * Extends the internal storage to
         * contain at least size more element
         */
        void ensure_can_add(size_type count) {
            auto current_size = size();
            auto max = max_size();

            // overflow
            if (max - count < current_size)
                throw std::length_error("Maximum size reached");

            auto new_size = count + current_size;

            if (new_size > the_capacity) {
                if (the_capacity > max / 2) {
                    force_reserve(max);
                } else {
                    // new capacity must be >= new_size
                    // so the_capacity * 2^power >= new_size
                    size_type power = ceil(log2( new_size / the_capacity ));
                    size_type new_capacity = the_capacity * pow(2, power);
                    force_reserve(new_capacity);
                }
            }
        }

        /**
         * Swaps inner contents of the two vectors
         */
        void raw_swap(vector && other) {
            std::swap(the_end, other.the_end);
            std::swap(the_begin, other.the_begin);
            std::swap(the_capacity, other.the_capacity);
            std::swap(the_allocator, other.the_allocator);
        }

        /**
         * Moves inner contents of the two vectors
         * into each others allocator space
         */
        void blyat_swap(vector && other) {
            // move our contents to others allocator
            vector to_them(std::move(*this), other.the_allocator);
            // move contents from other allocator to ours one
            vector to_us(std::move(other), the_allocator);
            // assign moved contents
            other.raw_swap(std::move(to_them));
            raw_swap(std::move(to_us));
        }
    };
}