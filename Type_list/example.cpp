/* EXAMPLE */
#include "type_list.hpp" 
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace types;
  using t1 = type_list<int, char, bool, type_list<float, double>, long>;
  std::cout << t1() << std::endl;
}
