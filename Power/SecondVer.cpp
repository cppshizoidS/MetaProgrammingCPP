#include <iostream>
#include <utility>

template <typename T, signed N> constexpr T power(const T base) {
  if constexpr (N < 0) {
    return 1 / power<T, -N>(base);
  }
  if constexpr (N == 0) {
    return 1;
  }
  if constexpr ((N & 1) == 0) {
    return power<T, N / 2>(base * base);
  }
  return base * power<T, N / 2>(base * base);
}

int main(int argc, char *argv[]) {
  constexpr auto m = power<int, 3>(-3);
  std::cout << m << std::endl; // output: -27

  constexpr auto n = power<int, -3>(-3);
  std::cout << n << std::endl; // output: 0

  constexpr auto p = power<double, 3>(2.5);
  std::cout << p << std::endl; // output: 15.625

  constexpr auto q = power<double, -3>(2.0);
  std::cout << q << std::endl; // output: 0.125
  return 0;
}
