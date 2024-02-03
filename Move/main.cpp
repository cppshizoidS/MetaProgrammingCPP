#include "my_move.hpp"
#include <utility>
#include <iostream>

auto main() -> int {
  int x = 42;
  int &&result = move(x);
  int &&result2 = std::move(x);

  std::cout << result;
  std::cout << result2;
}
