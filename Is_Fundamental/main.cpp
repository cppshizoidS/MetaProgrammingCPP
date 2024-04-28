#include "is_fundamental.hpp"
#include <print>
#include <string>
int main() {
  // Examples
  std::print("int is fundamental: {}\n", is_fundamental<int>::value);
  std::print("float is fundamental: {}\n", is_fundamental<float>::value);
  std::print("std::string is fundamental: {}\n",
             is_fundamental<std::string>::value);
  std::print("char is fundamental: {}\n", is_fundamental<char>::value);
  std::print("void is fundamental: {}\n", is_fundamental<void>::value);
  std::print("nullptr_t is fundamental: {}\n",
             is_fundamental<std::nullptr_t>::value);

  return 0;
}
