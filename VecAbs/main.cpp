#include <print>

#include "vec_abs.hpp"

int main() {
  vec::vec_abs<double, 3> vec1;
  std::array<double, 3> arr = {1.0, 2.0, 3.0};
  vec::vec_abs<double, 3> vec2(arr);
  vec::vec_abs<double, 3> vec3(4.0, 5.0, 6.0);

  bool convertible = vec::all_convertible_to<int, double, float>();

  std::print("Vector 1: ");
  for (const auto &elem : vec1.data) {
    std::print("{} ", elem);
  }
  std::print("\n");

  std::print("Vector 2: ");
  for (const auto &elem : vec2.data) {
    std::print("{} ", elem);
  }
  std::print("\n");

  std::print("Vector 3: ");
  for (const auto &elem : vec3.data) {
    std::print("{} ", elem);
  }
  std::print("\n");

  std::print("Are all types convertible to double? {}\n", convertible);
  return 0;
}
