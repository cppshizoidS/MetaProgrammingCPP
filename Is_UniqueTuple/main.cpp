#include <iostream>
#include "is_unique_tuple.hpp"

int main() {
    // Create a tuple with unique elements
    std::tuple<int, double, char> uniqueTuple {234, 234.34, 'a'};
    std::cout << "Is uniqueTuple unique? " << is_unique_tuple<decltype(uniqueTuple)>::value << std::endl;

    // Create a tuple with non-unique elements
    std::tuple<int, double, int> nonUniqueTuple;
    std::cout << "Is nonUniqueTuple unique? " << is_unique_tuple<decltype(nonUniqueTuple)>::value << std::endl;

    return 0;
}
