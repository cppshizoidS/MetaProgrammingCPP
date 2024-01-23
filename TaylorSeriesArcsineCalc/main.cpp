#include "asin.hpp"
#include <numbers>
#include <iostream>

int main() {
    constexpr double x = 0.5;
    constexpr int terms = 10;

    constexpr double asinApprox = taylorSeriesASin(x, terms);
    constexpr double asinDegrees = asinApprox * 180.0 / std::numbers::pi;

    static_assert(asinDegrees >= 25.999999999 && asinDegrees <= 30.000000001, "Incorrect approximation of asin(0.5)");

    std::cout << "Approximation of asin(" << x << ") using " << terms << " terms: " << asinDegrees << " degrees" << std::endl;


    return 0;
}

