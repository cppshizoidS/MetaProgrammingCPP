#ifndef ASIN_HPP
#define ASIN_HPP


#include <iostream>
#include <numbers>
#include <cmath>
#include <limits>
#include <type_traits>

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <FloatingPoint T>
constexpr T power(T base, int exponent) {
    return (exponent == 0) ? 1.0 : ((exponent % 2 == 0) ? power(base, exponent / 2) * power(base, exponent / 2) : base * power(base, exponent - 1));
}

template <typename T>
constexpr T factorial(int n) {
    return (n == 0) ? 1.0 : n * factorial<T>(n - 1);
}

template <FloatingPoint T>
constexpr T taylorSeriesASinImpl(T x, int n, int terms, T result) {
    return (n == terms) ? result : taylorSeriesASinImpl(x, n + 1, terms, result + (power<T>(-1, n) / factorial<T>(2 * n + 1)) * power(x, 2 * n + 1) / (2 * n + 1));
}

template <FloatingPoint T>
constexpr T taylorSeriesASin(T x, int terms) {
    return taylorSeriesASinImpl(x, 0, terms, 0.0);
}

constexpr double CHEBYSHEV_COEFF_0 = 1.0;
constexpr double CHEBYSHEV_COEFF_1 = -0.166667;
constexpr double CHEBYSHEV_COEFF_2 = 0.075;

template <FloatingPoint T>
constexpr T chebyshevASin(T x) {
    T xSquared = x * x;
    return x * (CHEBYSHEV_COEFF_0 + xSquared * (CHEBYSHEV_COEFF_1 + xSquared * CHEBYSHEV_COEFF_2));
}

constexpr bool checkAsinRange() {
    constexpr double x = 0.5;
    constexpr int terms = 10;
    return (taylorSeriesASin(x, terms) * 180.0 / std::numbers::pi >= 28.299999999 &&
            taylorSeriesASin(x, terms) * 180.0 / std::numbers::pi <= 30.000000001);
}

constexpr bool checkAsinChebRange() {
    constexpr double x = 0.5;
    return (chebyshevASin(x) * 180.0 / std::numbers::pi >= 28.299999999 &&
            chebyshevASin(x) * 180.0 / std::numbers::pi <= 30.000000001);
}
#endif // ASIN_HPP
