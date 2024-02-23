#include <iostream>
#include "is_specialization.hpp"

template <typename T, typename U>
class Pair {
public:
    T first;
    U second;
};

int main() {
    if (is_specialization_of_v<Pair<int, double>, Pair>) {
        std::cout << "Pair<int, double> is a specialization of Pair" << std::endl;
    } else {
        std::cout << "Pair<int, double> is NOT a specialization of Pair" << std::endl;
    }

    if (is_specialization_of_v<Pair<int, double>, Pair>) {
        std::cout << "Pair<int, double> is a specialization of Pair" << std::endl;
    } else {
        std::cout << "Pair<int, double> is NOT a specialization of Pair" << std::endl;
    }

    if (is_specialization_of_v<int, Pair>) {
        std::cout << "int is a specialization of Pair" << std::endl;
    } else {
        std::cout << "int is NOT a specialization of Pair" << std::endl;
    }

    return 0;
}
