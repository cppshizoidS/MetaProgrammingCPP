#include <array>
#include <iostream>

constexpr int TABLE_SIZE = 10;

template <int INDEX = 0, int... D>
struct Helper : Helper<INDEX + 1, D..., INDEX * INDEX> {};

template <int... D> struct Helper<TABLE_SIZE, D...> {
  static constexpr std::array<int, TABLE_SIZE> table = {D...};
};

constexpr std::array<int, TABLE_SIZE> table = Helper<>::table;

enum { FOUR = table[2] };

int main() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    std::cout << table[i] << std::endl;
  }
  std::cout << "FOUR: " << FOUR << std::endl;
}
