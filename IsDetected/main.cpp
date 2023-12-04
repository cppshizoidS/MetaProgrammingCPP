#include <type_traits>

template <typename...> using try_to_instantiate = void;

using disregard_this = void;

template <template <typename...> class Expression, typename Attempt,
          typename... Ts>
struct is_detected_impl : std::false_type {
  is_detected_impl(const is_detected_impl &) = delete;
  is_detected_impl(is_detected_impl &&) = delete;
  is_detected_impl &operator=(const is_detected_impl &) = delete;
  is_detected_impl &operator=(is_detected_impl &&) = delete;
};

template <template <typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, try_to_instantiate<Expression<Ts...>>,
                        Ts...> : std::true_type {};

template <template <typename...> class Expression, typename... Ts>
constexpr bool is_detected =
    is_detected_impl<Expression, disregard_this, Ts...>::value;

template <typename T, typename U>
using assign_expression = decltype(std::declval<T &>() = std::declval<U &>());

template <typename T, typename U>
constexpr bool is_assignable = is_detected<assign_expression, T, U>;
