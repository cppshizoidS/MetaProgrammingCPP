#include <array>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <type_traits>

#include "sorts.hpp"

template <typename T, std::size_t Size>
static std::ostream &operator<<(std::ostream &os, std::array<T, Size> array) {
    os << "{";
    for (auto it = array.cbegin(); it != array.cend();) {
        os << *it;
        if (++it != array.cend()) {
            os << ", ";
        }
    }
    os << "}";
    return os;
}

auto main(int argc, char **argv) -> int {
    using seq_original = metasort::sequence<int, 5, 7, 4, 2, 8, 6, 1, 9, 0, 3>;
    std::cout << " original: " << seq_original::to_array() << std::endl;

    using seq_quicksort = metasort::quicksort::sort_t<seq_original>;
    static_assert(
        std::is_same_v<seq_quicksort,
                       metasort::sequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>>,
        "quicksort assertion failed");
    std::cout << "quicksort: " << seq_quicksort::to_array() << std::endl;

    using seq_mergesort = metasort::mergesort::sort_t<seq_original>;
    static_assert(
        std::is_same_v<seq_mergesort,
                       metasort::sequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>>,
        "mergesort assertion failed");
    std::cout << "mergesort: " << seq_mergesort::to_array() << std::endl;
}