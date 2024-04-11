#include "fmap.hpp"
#include <print>
#include <ranges>

int square(int x) { return x * x; }

auto main() -> int {
  std::vector<int> numbers = {1, 2, 3, 4, 5};

  auto squared_numbers = fmap(square, numbers);

  for (const auto &n : squared_numbers) {
    std::print("{}", n);
  }
  std::println("{}", " ");
  return 0;
}
