#pragma once

#include <array>

template <typename T, std::size_t Capacity> struct static_vector {
  [[nodiscard]] constexpr auto operator[](std::size_t i) -> T & {
    return raw[i];
  }

  [[nodiscard]] constexpr auto operator[](std::size_t i) const -> T const & {
    return raw[i];
  }

  [[nodiscard]] constexpr auto size() const -> std::size_t { return count; }

  [[nodiscard]] constexpr auto data() const -> const T * { return raw.data(); }

  template <typename U> constexpr void emplace_back(U &&u) {
    raw[count] = static_cast<U>(u);
    count += 1;
  }

  std::array<T, Capacity> raw{};
  std::size_t count = 0;
};
