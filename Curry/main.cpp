#include "curry.hpp"
#include <iostream>

constexpr auto add = [](int x, int y) { return x + y; };

auto main() -> int {
  constexpr auto add5 = cppshizoid::tools::curry(add, 5);

  std::cout << add5(3) << std::endl;
  std::cout << add5(10) << std::endl;
}
