#ifndef VEC_ABS
#define VEC_ABS

#include <array>
#include <cstddef>
#include <print>
#include <type_traits>
#include <utility>

namespace vec {
template <typename T, typename... Ts>
constexpr bool all_convertible_to() {
  return (std::is_convertible_v<T, Ts> && ...);
}

template <typename T, size_t N>
class vec_abs {
 public:
  consteval vec_abs();

  [[maybe_unused]] constexpr vec_abs(const vec_abs &vec);

  [[maybe_unused]] constexpr vec_abs(vec_abs &&vec) noexcept;

  [[maybe_unused]] explicit constexpr vec_abs(const std::array<T, N> &arr);

  template <typename... Ts>
    requires std::conjunction_v<std::is_convertible<Ts, T>...> &&
             (sizeof...(Ts) == N)
  explicit consteval vec_abs(Ts &&...values);

  constexpr vec_abs &operator=(const vec_abs &other);

  constexpr vec_abs &operator=(vec_abs &&other) noexcept;

  std::array<T, N> data;
};

template <typename T, size_t N>
consteval vec_abs<T, N>::vec_abs() : data{} {}

template <typename T, size_t N>
[[maybe_unused]] constexpr vec_abs<T, N>::vec_abs(const vec_abs &vec)
    : data(vec.data) {}

template <typename T, size_t N>
[[maybe_unused]] constexpr vec_abs<T, N>::vec_abs(vec_abs &&vec) noexcept
    : data(std::move(vec.data)) {}

template <typename T, size_t N>
[[maybe_unused]] constexpr vec_abs<T, N>::vec_abs(const std::array<T, N> &arr)
    : data(arr) {}

template <typename T, size_t N>
template <typename... Ts>
  requires std::conjunction_v<std::is_convertible<Ts, T>...> &&
           (sizeof...(Ts) == N)
consteval vec_abs<T, N>::vec_abs(Ts &&...values)
    : data{std::forward<Ts>(values)...} {}

template <typename T, size_t N>
constexpr vec_abs<T, N> &vec_abs<T, N>::operator=(const vec_abs &other) {
  if (this != &other) {
    data = other.data;
  }
  return *this;
}

template <typename T, size_t N>
constexpr vec_abs<T, N> &vec_abs<T, N>::operator=(vec_abs &&other) noexcept {
  if (this != &other) {
    data = std::move(other.data);
  }
  return *this;
}
}  // namespace vec
#endif  // VEC_ABS
