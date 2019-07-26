#pragma once


/**
 * Custom implementations
 */
namespace my {
    /**
     * Allocator that prints messages
     * to std::cout whenever it's used.
     * None of the two instances of
     * this class are interchangable so
     * that any attempts to check whether
     * the two instances are equal to each other
     * will realize that they are not equal
     */
    template <typename T>
    struct debug_allocator {
        /**
         * Allows to access template type T
         */
        using value_type = T;

        /**
         * Generalizes memory menagement types
         */
        using       size_type = size_t;
        using difference_type = ptrdiff_t;

        /**
         * Generalizes memory menagement types
         */
        using       pointer =       value_type *;
        using const_pointer = const value_type *;

        /**
         * Generalizes memory menagement types
         */
        using       reference =       value_type &;
        using const_reference = const value_type &;

        /**
         * Returns the maximum possible count of elements
         */
        inline size_type max_size() const {
            return static_cast<size_type>(-1) / sizeof(T);
        }

        /**
         * Returns a pointer to the newly allocated memory
         */
        pointer allocate(size_type size, const_pointer = 0) {
            std::cout << "Allocating " << (size * sizeof(T)) << " bytes...\n";
            void * location = std::malloc(size * sizeof(T));

            if (!location) {
                throw std::bad_alloc();
            }

            return static_cast<pointer>(location);
        }

        /**
         * Deallocates space pointer to by location
         */
        void deallocate(pointer location, size_type = 0) {
            std::cout << "Deallocating...\n";
            std::free(location);
        }

        /**
         * Does nothing
         */
        ~debug_allocator() {}

        /**
         * Does nothing
         */
        debug_allocator() {}

        /**
         * Does nothing
         */
        debug_allocator(const debug_allocator<T> & other) {}

        /**
         * Allows implicit convertions between
         * allocators.
         */
        template <typename K>
        debug_allocator(const debug_allocator<K> & other) {}

        /**
         * Allows to access an allocator of a
         * different template type:
         * typename Got::template rebind<New>::other
         */
        template <typename K>
        struct rebind {
            using other = debug_allocator<K>;
        };

        /**
         * Not interchangable
         */
        bool operator == (const debug_allocator<T> & other) {
            return false;
        }

        /**
         * Not interchangable
         */
        bool operator != (const debug_allocator<T> & other) {
            return true;
        }
    };
}