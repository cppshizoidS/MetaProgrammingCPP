#ifndef ASIN_HPP
#define ASIN_HPP

#include <cmath>
#include <limits>
#include <ranges>
#include <type_traits>

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <FloatingPoint T> constexpr T power(T base, int exponent) {
  if (exponent == 0) {
    return 1.0;
  } else if (exponent % 2 == 0) {
    T halfPower = power(base, exponent / 2);
    return halfPower * halfPower;
  } else {
    return base * power(base, exponent - 1);
  }
}

template <typename T>
constexpr T power(T base, int exponent)
  requires std::is_integral_v<T>
{
  T result = 1;
  for (int i : std::views::iota(0, exponent)) {
    result *= base;
  }
  return result;
}

template <FloatingPoint T> constexpr T factorial(int n) {
  T result = 1.0;
  for (int i : std::views::iota(1, n)) {
    result *= i;
  }
  return result;
}

template <FloatingPoint T> constexpr T taylorSeriesASin(T x, int terms) {
  T result = 0.0;
  for (int n : std::views::iota(0, terms)) {
      T term = factorial<T>(2 * n - 1) /factorial<T>(2 * n) * power(x, 2*n+1) / (2*n+1);
      if (std::abs(term) < std::numeric_limits<T>::epsilon() * std::abs(result + term)) {
          break;
      }
      result += term;
    }
    return result;
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
