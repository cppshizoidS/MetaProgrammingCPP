#ifndef ASIN_HPP
#define ASIN_HPP

#include <type_traits>
#include <ranges>

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <FloatingPoint T>
constexpr T power(T base, int exponent) {
    T result = 1.0;
    for (int i : std::views::iota(0, exponent)) {
        result *= base;
    }
    return result;
}

template <typename T>
constexpr T power(T base, int exponent) requires std::is_integral_v<T> {
    T result = 1;
    for (int i : std::views::iota(0, exponent)) {
        result *= base;
    }
    return result;
}

template <FloatingPoint T>
constexpr T factorial(int n) {
    T result = 1.0;
    for (int i : std::views::iota(1, n + 1)) {
        result *= i;
    }
    return result;
}

template <FloatingPoint T>
constexpr T taylorSeriesASin(T x, int terms) {
    T result = 0.0;
    for (int n : std::views::iota(0, terms)) {
        result += (power<T>(-1, n) / factorial<T>(2 * n + 1)) * power(x, 2 * n + 1);
    }
    return result;
}

#endif // ASIN_HPP
