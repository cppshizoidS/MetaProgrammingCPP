#include <iostream>
#include "constexpr_tuple_accumulate.hpp" 
int main() {
  // create tuple
  constexpr auto my_tuple = std::make_tuple(1, 2, 3, 4, 5);

  //count sum in compile time
  constexpr auto sum = lib::constexpr_tuple_accumulate(my_tuple);

  std::cout << "Sum of tuple elements: " << sum << std::endl;

  return 0;
}
