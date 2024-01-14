#ifndef LAMBDA_COROS_HPP
#define LAMBDA_COROS_HPP

#include <cstddef>
#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

namespace cppshizoid::lambda_coros {

constexpr void lambda_co_concatenate_impl(char *dest, const char *a,
                                          const char *b) {
  while (*a) {
    *dest++ = *a++;
  }
  while (*b) {
    *dest++ = *b++;
  }
  *dest = '\0';
}

constexpr auto lambda_co_concatenate(const char *a, const char *b) {
  char result[256]; // Adjust the size as needed
  lambda_co_concatenate_impl(result, a, b);
  return std::string(result);
}

constexpr auto lambda_co_concatenate_line() {
  return lambda_co_concatenate("long", "");
}
template <typename ParamType>
constexpr void lambda_co_status_check(ParamType &value) {
  static_assert(std::is_integral_v<ParamType>, "integral state is required");
  static_assert(!std::is_const_v<ParamType>, "mutable lambda required");
  static_assert(sizeof(value) >= 4, "state must be at least 32 bits");
}

inline constexpr std::size_t lambda_co_constexpr_line = 0;

template <typename Lambda>
constexpr auto lambda_co_range(Lambda lambda, std::size_t skip = 0,
                               std::optional<std::size_t> length = {},
                               std::size_t stride = 1) {
  struct Range {
    Lambda generator;
    std::optional<std::size_t> length;
    std::size_t stride;
    std::size_t position = 0;

    struct Iterator {
      std::reference_wrapper<Lambda> lambda;
      std::reference_wrapper<std::size_t> position;
      std::optional<std::size_t> length;
      std::reference_wrapper<std::size_t> stride_ref;
      bool is_end = false;

      constexpr bool has_more() const noexcept {
        if (length && (position.get() >= length.value())) {
          return false;
        }
        return true;
      }

      constexpr bool increment_position() {
        if (has_more()) {
          ++position.get();
          return true;
        } else {
          return false;
        }
      }

      constexpr auto &operator++() {
        for (std::size_t i = 1; i < stride_ref.get(); ++i) {
          if (increment_position()) {
            lambda();
          }
        }
        return *this;
      }

      constexpr bool operator==(const Iterator &rhs) const {
        if (is_end && !rhs.has_more()) {
          return true;
        }
        if (rhs.is_end && !rhs.has_more()) {
          return true;
        }
        if (rhs.is_end && is_end) {
          return true;
        }
        return false;
      }

      constexpr bool operator!=(const Iterator &rhs) const {
        return !(*this == rhs);
      }

      constexpr decltype(auto) operator*() {
        increment_position();
        return lambda();
      }
    };

    constexpr auto begin() noexcept {
      return Iterator{generator, position, length, stride, false};
    }

    constexpr auto end() noexcept {
      return Iterator{generator, position, length, stride, true};
    }
  };

  for (std::size_t i = 0; i < skip; ++i) {
    lambda();
  }
  return Range{std::move(lambda), length, stride};
}

template <typename Lambda> constexpr auto while_has_value(Lambda lambda) {
  struct Range {
    Lambda generator;

    struct Iterator {
      std::reference_wrapper<Lambda> lambda;
      std::invoke_result_t<Lambda> current_value{};

      Iterator(Lambda &lambda_, bool is_end_)
          : lambda(lambda_), current_value([&]() {
              return is_end_ ? decltype(current_value){} : lambda();
            }()) {}

      constexpr auto &operator++() {
        current_value = lambda();
        return *this;
      }

      constexpr bool operator==(const Iterator &rhs) const {
        if (!current_value && !rhs.current_value) {
          return true;
        }
        return false;
      }

      constexpr bool operator!=(const Iterator &rhs) const {
        return !(*this == rhs);
      }

      constexpr decltype(auto) operator*() { return current_value.value(); }
    };

    constexpr auto begin() noexcept { return Iterator{generator, false}; }

    constexpr auto end() noexcept { return Iterator{generator, true}; }
  };

  return Range{std::move(lambda)};
}
} // namespace cppshizoid::lambda_coros

#endif // !LAMBDA_COROS_HPP
