#include <iostream>
#include <utility>
//compile time power
template <class T, T V, T N, class I = std::make_integer_sequence<T, N>>
struct power;

template <class T, T V, T N, T... Is>
struct power<T, V, N, std::integer_sequence<T, Is...>> {
  static constexpr T value =
      (static_cast<T>(1) * ... * (V * static_cast<bool>(Is + 1)));
};

template <typename T, signed N>
constexpr T powerV2(const T base) {
    if constexpr (N < 0) {
        return 1 / powerV2<T, -N>(base);
    }
    if constexpr (N == 0) {
        return 1;
    }
    if constexpr ((N & 1) == 0) {
        return powerV2<T, N / 2>(base * base);
    }
    return base * powerV2<T, N / 2>(base * base);
}

int main() {
    std::cout << "First version";
    std::cout << power<int, 2, 4>::value << std::endl; 

    std::cout << "Second version";

    constexpr auto m = powerV2<int, 3>(-3);
    std::cout << m << std::endl; // output: -27

    constexpr auto n = powerV2<int, -3>(-3);
    std::cout << n << std::endl; // output: 0

    constexpr auto p = powerV2<double, 3>(2.5);
    std::cout << p << std::endl; // output: 15.625

    constexpr auto q = powerV2<double, -3>(2.0);
    std::cout << q << std::endl; // output: 0.125

    return 0;
}
