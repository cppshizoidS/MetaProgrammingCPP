#include "intersection.hpp"
#include <iostream>

template <typename... Args> struct TypeList {};

int main() {
  using List1 = TypeList<int, float, double, char>;
  using List2 = TypeList<float, char, bool>;

  using Result = intersection::intersection<List1, List2>;

  // Result should be TypeList<float, char>
  return 0;
}
