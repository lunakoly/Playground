#pragma once


/**
 * Custom implementations
 */
namespace my {
    /**
     * Extends T type by one byte.
     * If T is empty than sizeof(not_empty_wrapper<T>)
     * will be the same as sizeof it's contents (char)
     */
    template <typename T>
    struct not_empty_wrapper : T {
        static const char addon = '1';
    };

    /**
     * Use is_empty<T>::value to find out
     * if T is empty
     */
    template <typename T>
    struct is_empty {
        static const bool value = sizeof(not_empty_wrapper<T>) == sizeof(char);
    };


    /**
     * Base will act as a set of particular
     * implementations
     */
    template <typename T, typename K, bool IsFirstEmpty, bool IsSecondEmpty>
    class compressed_pair_base;

    /**
     * No empties
     */
    template <typename T, typename K>
    class compressed_pair_base<T, K, false, false> {
    public:
        /**
         * Allows to access template types.
         * Despite these definitions exist I prefer
         * using T and K.
         */
        using  first_type = T;
        using second_type = K;

        /**
         * Generalizes memory menagement types.
         * TODO: call_traits
         */
        using        first_reference =       T &;
        using       second_reference =       K &;
        using  first_const_reference = const T &;
        using second_const_reference = const K &;

        /**
         * Instantiates default T and K
         */
        compressed_pair_base() {}

        /**
         * Instantiates first and second as copies
         */
        compressed_pair_base(const T & first, const K & second)
            : the_first(first), the_second(second) {}

        /**
         * Instantiates first as copy
         */
        compressed_pair_base(const T & first)
            : the_first(first) {}

        /**
         * Instantiates second as copy
         */
        compressed_pair_base(const K & second)
            : the_second(second) {}

        /**
         * Returns the first
         */
        first_reference first() {
            return the_first;
        }

        /**
         * Returns the first
         */
        first_const_reference first() const {
            return the_first;
        }

        /**
         * Returns the second
         */
        second_reference second() {
            return the_second;
        }

        /**
         * Returns the second
         */
        second_const_reference second() const {
            return the_second;
        }

    private:
        T the_first;
        K the_second;
    };

    /**
     * Most common situation where the one is empty
     */
    template <typename T, typename K>
    class compressed_pair_base<T, K, true, false> : T {
    public:
        /**
         * Allows to access template types.
         * Despite these definitions exist I prefer
         * using T and K.
         */
        using  first_type = T;
        using second_type = K;

        /**
         * Generalizes memory menagement types.
         * TODO: call_traits
         */
        using        first_reference =       T &;
        using       second_reference =       K &;
        using  first_const_reference = const T &;
        using second_const_reference = const K &;

        /**
         * Instantiates default T and K
         */
        compressed_pair_base() {}

        /**
         * Instantiates first and second as copies
         */
        compressed_pair_base(const T & first, const K & second)
            : T(first), the_second(second) {}

        /**
         * Instantiates first as copy
         */
        compressed_pair_base(const T & first)
            : T(first) {}

        /**
         * Instantiates second as copy
         */
        compressed_pair_base(const K & second)
            : the_second(second) {}

        /**
         * Returns the first
         */
        first_reference first() {
            return *this;
        }

        /**
         * Returns the first
         */
        first_const_reference first() const {
            return *this;
        }

        /**
         * Returns the second
         */
        second_reference second() {
            return the_second;
        }

        /**
         * Returns the first
         */
        second_const_reference second() const {
            return the_second;
        }

    private:
        K the_second;
    };

    /**
     * Mirror Empty and non empty if possible
     */
    template <typename T, typename K>
    struct compressed_pair_base<T, K, false, true> : compressed_pair_base<K, T, true, false> {};

    /**
     * Since first and second are the two different
     * instances we have to instantiate at least one of them
     */
    template <typename T, typename K>
    struct compressed_pair_base<T, K, true, true> : compressed_pair_base<T, K, true, false> {};

    /**
     * No empties but types are the same.
     * It required a separate implementation
     * because of overlapping constructor definitions
     */
    template <typename T>
    class compressed_pair_base<T, T, false, false> {
    public:
        /**
         * Allows to access template types.
         * Despite these definitions exist I prefer
         * using T.
         */
        using  first_type = T;
        using second_type = T;

        /**
         * Generalizes memory menagement types.
         * TODO: call_traits
         */
        using        first_reference =       T &;
        using       second_reference =       T &;
        using  first_const_reference = const T &;
        using second_const_reference = const T &;

        /**
         * Instantiates default Ts
         */
        compressed_pair_base() {}

        /**
         * Instantiates first and second as copies
         */
        compressed_pair_base(const T & first, const T & second)
            : the_first(first), the_second(second) {}

        /**
         * Instantiates first and second as copies
         */
        compressed_pair_base(const T & value)
            : the_first(value), the_second(value) {}

        /**
         * Returns the first
         */
        first_reference first() {
            return the_first;
        }

        /**
         * Returns the first
         */
        first_const_reference first() const {
            return the_first;
        }

        /**
         * Returns the second
         */
        second_reference second() {
            return the_second;
        }

        /**
         * Returns the second
         */
        second_const_reference second() const {
            return the_second;
        }

    private:
        T the_first;
        T the_second;
    };

    /**
     * Both are empty and types are the same.
     * It required a separate implementation
     * because of overlapping constructor definitions
     */
    template <typename T>
    class compressed_pair_base<T, T, true, true> : T {
    public:
        /**
         * Allows to access template types.
         * Despite these definitions exist I prefer
         * using T.
         */
        using  first_type = T;
        using second_type = T;

        /**
         * Generalizes memory menagement types.
         * TODO: call_traits
         */
        using        first_reference =       T &;
        using       second_reference =       T &;
        using  first_const_reference = const T &;
        using second_const_reference = const T &;

        /**
         * Instantiates default Ts
         */
        compressed_pair_base() {}

        /**
         * Instantiates first and second as copies
         */
        compressed_pair_base(const T & first, const T & second)
            : T(first), the_second(second) {}

        /**
         * Instantiates first and second as copies
         */
        compressed_pair_base(const T & value)
            : T(value), the_second(value) {}

        /**
         * Returns the first
         */
        first_reference first() {
            return *this;
        }

        /**
         * Returns the first
         */
        first_const_reference first() const {
            return *this;
        }

        /**
         * Returns the second
         */
        second_reference second() {
            return the_second;
        }

        /**
         * Returns the second
         */
        second_const_reference second() const {
            return the_second;
        }

    private:
        T the_second;
    };

    /**
     * The main user interface that selects
     * the proper implementation
     */
    template <typename T, typename K>
    struct compressed_pair : compressed_pair_base<T, K, is_empty<T>::value, is_empty<K>::value> {};
}