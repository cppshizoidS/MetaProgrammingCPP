#include "get_element.hpp"
#include <iostream>

int main() {
  std::cout << lib::get_element<2>(42, 'c', 3.14, true) << '\n';
  return 0;
}
