#include <iostream>

template <class T, T V, T N, class I = std::make_integer_sequence<T, N>>
struct power;

template <class T, T V, T N, T... Is>
struct power<T, V, N, std::integer_sequence<T, Is...>> {
  static constexpr T value =
      (static_cast<T>(1) * ... * (V * static_cast<bool>(Is + 1)));
};

int main() { std::cout << power<int, 2, 4>::value << std::endl; }
