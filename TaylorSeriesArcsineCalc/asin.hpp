#ifndef ASIN_HPP
#define ASIN_HPP

#include <cmath>
#include <limits>
#include <ranges>
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
    return (n == terms) ? result : taylorSeriesASinImpl(x, n + 1, terms, result + (factorial<T>(2 * n - 1) / factorial<T>(2 * n)) * power(x, 2 * n + 1) / (2 * n + 1));
}

template <FloatingPoint T>
constexpr T taylorSeriesASin(T x, int terms) {
    return taylorSeriesASinImpl(x, 0, terms, 0.0);
}

constexpr double chebyshevCoeff0 = 1.0;
constexpr double chebyshevCoeff1 = -0.166667;
constexpr double chebyshevCoeff2 = 0.075;

template <FloatingPoint T> constexpr T chebyshevASin(T x) {
  T xSquared = x * x;
  T result = x * (chebyshevCoeff0 + xSquared * (chebyshevCoeff1 + xSquared * chebyshevCoeff2));
  return result;
}

constexpr bool checkAsinRange() {
  constexpr double x = 0.5;
  constexpr int terms = 10;
  double asinApprox = taylorSeriesASin(x, terms);
  double asinDegrees = asinApprox * 180.0 / std::numbers::pi;

  return (asinDegrees >= 28.599999999 && asinDegrees <= 30.000000001);
}

constexpr bool checkAsinChebRange() {
  constexpr double x = 0.5;
  constexpr double chebasinApprox = chebyshevASin(x);
  double chebasinDegrees = std::asin(x) * 180.0 / std::numbers::pi;
  return (chebasinDegrees >= 28.599999999 && chebasinDegrees <= 30.000000001);
}

#endif // ASIN_HPP
