#include <iostream>

// template meta recursive function
template <unsigned int n> struct _Factorial {
  enum { value = n * _Factorial<n - 1>::value };
};

// base
template <> struct _Factorial<0> {
  enum { value = 1 };
};

// wrapper
#define factorial(x) _Factorial<x>::value

int main() {

  std::cout << factorial(0) << std::endl;
  std::cout << factorial(1) << std::endl;
  std::cout << factorial(4) << std::endl;
  std::cout << factorial(8) << std::endl;

  return 0;
}
