#include "vec_abs.hpp"

template <typename T, size_t N> vec_abs<T, N>::vec_abs() {
  data = std::array<T, N>();
}

template <typename T, size_t N> vec_abs<T, N>::vec_abs(const vec_abs &vec) {
  this = vec;
}

template <typename T, size_t N>
vec_abs<T, N>::vec_abs(const std::array<T, N> &arr) {
  data = arr;
}

template <typename T, size_t N>
template <typename... Ts>
  requires std::conjunction<std::is_convertible<Ts, T>...>::value &&
           (sizeof...(Ts) == N)
vec_abs<T, N>::vec_abs(const Ts &&...values) {}
