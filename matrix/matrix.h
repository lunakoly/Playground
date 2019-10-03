#pragma once


/**
 * Custom implementations
 */
namespace my {
    /**
     * Represents the shape of some matrix
     */
    template <typename size_type>
    struct matrix_shape {
        /**
         * The number of rows
         */
        size_type rows = 0;

        /**
         * The number of columns
         */
        size_type columns = 0;

        /**
         * Returns rows * columns
         */
        size_type size() const noexcept {
            return rows * columns;
        }
    };
}