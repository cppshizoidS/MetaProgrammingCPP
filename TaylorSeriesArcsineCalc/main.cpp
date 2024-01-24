#include "asin.hpp"
#include <numbers>
auto main() -> int {
  constexpr double x = 0.5;
  constexpr int terms = 10;

  constexpr double asinApprox = taylorSeriesASin(x, terms);
  constexpr double asinDegrees = asinApprox * 180 / std::numbers::pi;

  constexpr double chebasinApprox = chebyshevASin(x);
  double chebasinDegrees = std::asin(x) * 180.0 / std::numbers::pi;

  printf("Taylor Approximation of asin(0.5): %f\n", asinApprox);
  printf("Asin(0.5) in degrees: %f\n", asinDegrees);
  static_assert(checkAsinRange(), "Incorrect approximation of asin(0.5)");

  printf("Chebash Approximation of asin(0.5): %f\n", chebasinApprox);
  printf("Asin(0.5) in degrees: %f\n", chebasinDegrees);

}
