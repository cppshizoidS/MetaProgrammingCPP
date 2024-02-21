#ifndef IS_PRINTABLE
#define IS_PRINTABLE

#include <concepts>
#include <iostream>

namespace is_printable {

template <typename T>
concept Printable = requires(T value) {
  { std::cout << value } -> std::same_as<std::ostream &>;
};

template <typename T> void printTwice(const T &value) {
  std::cout << value << " ";
  std::cout << value << std::endl;
}
} // namespace is_printable
#endif // IS_PRINTABLE
