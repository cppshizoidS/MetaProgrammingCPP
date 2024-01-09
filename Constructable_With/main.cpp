#include "constructable_with.hpp"
#include <iostream>
struct Person {
  std::string name;
  int age;
};

struct Book {
  std::string title;
  std::string author;
  int year;
};

int main() {
  // is person is aggrefate of
  if constexpr (lib::aggregate_of<Person,
                                  lib::number_of_aggregate_members<Person>>) {
    std::cout << "Struct Person is aggregate of" << std::endl;
  } else {
    std::cout << "Struct Person is not aggregate of" << std::endl;
  }

  // is boook is aggrefate of
  if constexpr (lib::aggregate_of<Book,
                                  lib::number_of_aggregate_members<Book>>) {
    std::cout << "Struct Book is aggregate of" << std::endl;
  } else {
    std::cout << "Struct Book is not aggregate of" << std::endl;
  }

  return 0;
}
