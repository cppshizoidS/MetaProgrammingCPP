#include "lambda_utils.hpp"
#include <iostream>

int main() {
  int state_range = 0;

  auto lambda_range = [&state_range]() mutable { return state_range++; };

  auto range =
      cppshizoid::lambda::lambda_range(lambda_range, 0, 5, 2);

  for (const auto &value : range) {
    std::cout << value << " ";
  }

  std::cout << std::endl;

  int state_while = 0;

  auto lambda_while = [&state_while]() mutable -> std::optional<int> {
    cppshizoid::lambda::lambda_status_check(
        state_while); 
    if (state_while < 5) {
      return state_while++;
    } else {
      return {}; // End of values
    }
  };

  auto range_while =
      cppshizoid::lambda::while_has_value(lambda_while);

  for (const auto &value : range_while) {
    std::cout << value << " ";
  }

  std::cout << std::endl;

  return 0;
}
