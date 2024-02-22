#include <iostream>
#include "is_specialization.hpp" 

// example
template <typename T>
struct MyTemplate {};

int main() {
    // Check if MyTemplate<int> is specialization MyTemplate
    if (is_specialization_of<MyTemplate<int>, MyTemplate>::value) {
        std::cout << "MyTemplate<int> is a specialization of MyTemplate." << std::endl;
    } else {
        std::cout << "MyTemplate<int> is not a specialization of MyTemplate." << std::endl;
    }

    return 0;
}
