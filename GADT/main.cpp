#include "gadt.hpp"
#include <print>

auto main() -> int {
  auto expr = Mul{Add{Lit<1>{}, Lit<2>{}}, Lit<3>{}};
  int result = eval(expr);
  std::print("{}", result);
  return 0;
}
