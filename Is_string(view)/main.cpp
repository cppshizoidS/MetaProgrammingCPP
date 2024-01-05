#include <string>
#include <string_view>
#include <type_traits>

template <typename T> struct is_string : std::false_type {};

template <> struct is_string<std::string> : std::true_type {};

template <typename T> inline constexpr bool is_string_v = is_string<T>::value;

template <typename T> struct is_string_view : std::false_type {};

template <> struct is_string_view<std::string_view> : std::true_type {};

template <typename T>
inline constexpr bool is_string_view_v = is_string_view<T>::value;

auto main() -> int {
  static_assert(is_string_v<std::string>);
  static_assert(!is_string_v<int>);

  static_assert(is_string_view_v<std::string_view>);
  static_assert(!is_string_view_v<std::string_view>);//error because is string_view
}
