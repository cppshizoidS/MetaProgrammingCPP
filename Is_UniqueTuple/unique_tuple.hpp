#ifndef UNIQUE_TUPLE_HPP
#define UNIQUE_TUPLE_HPP

#include <tuple>
#include <type_traits>

template<typename T, typename Tuple>
struct is_unique_tuple_helper;

template<typename T, typename... Ts>
struct is_unique_tuple_helper<T, std::tuple<Ts...>> {
    static constexpr bool value = (std::is_same_v<T, Ts> && ...) && (sizeof...(Ts) == 0 || !is_unique_tuple_helper<T, std::tuple<Ts...>>::value);
};

template<typename Tuple>
struct is_unique_tuple : is_unique_tuple_helper<void, Tuple> {};

#endif // UNIQUE_TUPLE_HPP
