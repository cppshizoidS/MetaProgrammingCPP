#include "count_if.hpp"
#include <iostream>
#include <string>

void example_with_tuple() {
  std::tuple<int, double, std::string, char> myTuple(42, 3.14, "Hello", 'a');

  constexpr auto countIntegral = cppshizoid::count_if(
      [](auto &&value) {
        return std::is_integral_v<std::decay_t<decltype(value)>>;
      },
      myTuple);

  std::cout << "Number of integral elements in the tuple: " << countIntegral
            << std::endl;
}

void example_with_arguments() {
  constexpr auto countFloatingPoint = cppshizoid::count_if(
      [](auto &&value) {
        return std::is_floating_point_v<std::decay_t<decltype(value)>>;
      },
      3.14, 2.718, 42.0);

  std::cout << "Number of floating-point arguments: " << countFloatingPoint
            << std::endl;
}

int main() {
  example_with_tuple();
  example_with_arguments();

  return 0;
}
