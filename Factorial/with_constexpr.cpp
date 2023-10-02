#include <iostream>
#include <iterator>

constexpr int factorial(int n) { return n <= 1 ? 1 : (n * factorial(n - 1)); }

int main() {
  std::cout << factorial(0) << std::endl;
  std::cout << factorial(1) << std::endl;
  std::cout << factorial(4) << std::endl;
  std::cout << factorial(8) << std::endl;
  return 0;
}
