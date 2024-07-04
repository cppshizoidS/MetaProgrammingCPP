#include <tuple>
#include <string>
#include <cstdlib>
#include <utility>
#include <print>
#include <type_traits>

template <std::size_t _Idx, typename _From, typename _To, typename... _Ts>
constexpr auto replace_tuple_type_at(const std::tuple<_Ts...>& tuple, const _To& value) {
  if constexpr (std::is_same_v<_From,
          std::tuple_element_t<_Idx, std::tuple<_Ts...>>>)
    return value;
  else
    return std::get<_Idx>(tuple);
}

template <typename _From, typename _To, typename... _Ts, std::size_t... _Is>
constexpr auto replace_tuple_types_impl(const std::tuple<_Ts...>& tuple,
  const _To& value, std::index_sequence<_Is...>) {
  return std::make_tuple(
    replace_tuple_type_at<_Is, _From, _To>(tuple, value)...
  );
}

template <typename _From, typename _To, typename... _Ts>
constexpr auto replace_tuple_types(const std::tuple<_Ts...>& tuple, const _To& value) {
  return replace_tuple_types_impl<_From, _To>(
    tuple, value, std::make_index_sequence<sizeof...(_Ts)>()
  );
}

auto main() -> int {
  constexpr std::tuple tuple1(1, 2.0, 1);
  std::tuple tuple2 = replace_tuple_types<int, std::string>(tuple1, "str");

  std::print("{},{},{}\n", std::get<0>(tuple2), std::get<1>(tuple2), std::get<2>(tuple2));

  return 0;
}
