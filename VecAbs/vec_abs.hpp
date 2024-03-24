#ifndef VEC_ABS
#define VEC_ABS

#include <array>
#include <cstddef>
#include <type_traits>

template <typename T, typename... Ts> constexpr bool all_convertible_to() {
  return (std::is_convertible_v<T, Ts> && ...);
}

template <typename T, size_t N> class vec_abs {
public:
  vec_abs();                   // construct from another vector
  vec_abs(const vec_abs &vec); // construct from another vector
  explicit vec_abs(const std::array<T, N> &arr); // construct from array

  template <typename... Ts>
    requires std::conjunction<std::is_convertible<Ts, T>...>::value &&
             (sizeof...(Ts) == N)         
  explicit vec_abs(const Ts &&...values); // construct from multiple arguments

  std::array<T, N> data;
};
#endif
