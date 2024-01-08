#include "is_smart_pointer.hpp"
#include <iostream>
#include <memory>
int main() {
  std::cout << "std::unique_ptr<int> is a smart pointer: "
            << is_smart_pointer<std::unique_ptr<int>>::value << std::endl;
  std::cout << "std::shared_ptr<int> is a smart pointer: "
            << is_smart_pointer<std::shared_ptr<int>>::value << std::endl;
  std::cout << "int* is a smart pointer: " << is_smart_pointer<int *>::value
            << std::endl;

  return 0;
}
