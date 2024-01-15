#include "lambda.hpp"

#include <iostream>

int main() {
  // Define a lambda coroutine to generate a range of numbers
  auto number_range = cppshizoid::lambda_coroutines::range([]() -> int {
    static int value = 0;
    return value++;
  }, 0, 5, 2);  // Generate a range from 0 to 5 with a stride of 2

  // Iterate over the generated range using a lambda coroutine
  for (auto value : number_range) {
    std::cout << "Current value: " << value << '\n';
  }

  // Define a lambda coroutine to generate values while a condition is true
  auto values_while_true = cppshizoid::lambda_coroutines::while_has_value([]() -> std::optional<int> {
    static int value = 1;
    if (value <= 5) {
      return value++;
    } else {
      return std::nullopt;  // Signal the end of values
    }
  });

  // Iterate over the generated values while the condition is true
  for (auto value : values_while_true) {
    std::cout << "Current value: " << value << '\n';
  }

  // Example 1: Concatenating two lambdas that print messages
    auto lambda1 = [] { std::cout << "Hello, "; };
    auto lambda2 = [] { std::cout << "World!" << std::endl; };

    auto concatenatedLambda1 = cppshizoid::lambda_coroutines::lambda_co_concatenate(lambda1, lambda2);
    concatenatedLambda1();  // Output: Hello, World!

    // Example 2: Concatenating two lambdas with parameters
    auto lambda3 = [](int x) { std::cout << "Value: " << x << std::endl; };
    auto lambda4 = [](double y) { std::cout << "Decimal: " << y << std::endl; };

    auto concatenatedLambda2 = cppshizoid::lambda_coroutines::lambda_co_concatenate(lambda3, lambda4);
    concatenatedLambda2(42);        // Output: Value: 42
    concatenatedLambda2(3.14);      // Output: Decimal: 3.14

  return 0;
}
