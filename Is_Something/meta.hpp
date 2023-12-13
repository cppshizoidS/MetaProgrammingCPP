#ifndef METAPROGRAMMING_HPP
#define METAPROGRAMMING_HPP

#include <cstddef>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace meta {
// type identiy
template<typename T>
struct type_identity {
    using type = T;
};
// index constans
template<std::size_t idx>
using index_constant = std::integral_constant<std::size_t, idx>;

template<std::size_t t_first, std::size_t t_second>
struct index_pair {
    constexpr inline static std::size_t first{t_first};
    constexpr inline static std::size_t second{t_second};
};
// sum type
template<typename... Ts>
using sum_type = type_identity<decltype((... + std::declval<Ts>()))>;
// sum_type alias
template<typename... Ts>
using sum_type_t = typename sum_type<Ts...>::type;
// remove_cvref
template<typename T>
using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;
// is type in pack
template<typename T, typename... Pack>
struct is_type_in_pack
    : std::bool_constant<(std::is_same_v<T, Pack> || ...)> {};

template<typename T>
struct is_type_in_pack<T> : std::false_type {};

template<typename T, typename... Pack>
constexpr inline bool is_type_in_pack_v = is_type_in_pack<T, Pack...>::value;
// peel first
template<typename... Pack>
struct peel_first : type_identity<void> {};

template<typename First, typename... Pack>
struct peel_first<First, Pack...> : type_identity<First> {};
// peel fisrt alias
template<typename... Pack>
using peel_first_t = typename peel_first<Pack...>::type;
// peel last
template<typename... Pack>
struct peel_last : type_identity<void> {};

template<typename First, typename... Pack>
struct peel_last<First, Pack...>
    : type_identity<typename peel_last<Pack...>::type> {};

template<typename Last>
struct peel_last<Last> : type_identity<Last> {};
// peel last alias
template<typename... Pack>
using peel_last_t = typename peel_last<Pack...>::type;
// is pack uniform
template<typename... Pack>
struct is_pack_uniform {};

template<typename T, typename... Pack>
struct is_pack_uniform<T, Pack...>
    : std::bool_constant<(std::is_same_v<T, Pack> && ...)> {};
// is pack uniform alias
template<typename... Pack>
constexpr inline bool is_pack_uniform_v = is_pack_uniform<Pack...>::value;
// is pack only
template<typename T, typename... Pack>
struct is_pack_only
    : std::conjunction<
          is_pack_uniform<Pack...>,
          std::is_same<T, peel_first_t<Pack...>>> {};
// is pack only alias
template<typename T>
struct is_pack_only<T> : std::false_type {};

template<typename T, typename... Pack>
constexpr inline bool is_pack_only_v = is_pack_only<T, Pack...>::value;
// is iterable
template<typename T, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<
    T,
    std::void_t<
        decltype(std::begin(std::declval<T&>())),
        decltype(std::end(std::declval<T&>()))>> : std::true_type {};
// is iterable alias
template<typename T>
constexpr inline bool is_iterable_v = is_iterable<T>::value;
// is iterator
template<typename T, typename = void>
struct is_iterator : std::false_type {};

template<typename T>
struct is_iterator<
    T,
    std::void_t<decltype(*std::declval<T>()), decltype(++std::declval<T&>())>>
    : std::true_type {};
// is iterator alias
template<typename T>
constexpr inline bool is_iterator_v = is_iterator<T>::value;
// is forward
template<typename T>
struct is_forward
    : std::is_base_of<
          std::forward_iterator_tag,
          typename std::iterator_traits<T>::iterator_category> {};
// is forward alias
template<typename T>
constexpr inline bool is_forward_v = is_forward<T>::value;
// is bidirection
template<typename T>
struct is_bidirectional
    : std::is_base_of<
          std::bidirectional_iterator_tag,
          typename std::iterator_traits<T>::iterator_category> {};
// is bidirectional alias
template<typename T>
constexpr inline bool is_bidirectional_v = is_bidirectional<T>::value;
// is random access
template<typename T>
struct is_random_access
    : std::is_base_of<
          std::random_access_iterator_tag,
          typename std::iterator_traits<T>::iterator_category> {};
// is random access aliaas
template<typename T>
constexpr inline bool is_random_access_v = is_random_access<T>::value;

namespace impl {

    template<typename... Ts>
    struct is_tuple_impl : std::false_type {};

    template<typename... Ts>
    struct is_tuple_impl<std::tuple<Ts...>> : std::true_type {};
}
// is tuple
template<typename T>
struct is_tuple : impl::is_tuple_impl<remove_cvref_t<T>> {};
// is tuple aslias
template<typename T>
constexpr inline bool is_tuple_v = is_tuple<T>::value;

namespace impl {
    // partial impl
    template<typename T>
    struct is_pair_impl : std::false_type {};

    template<typename First, typename Second>
    struct is_pair_impl<std::pair<First, Second>> : std::true_type {};
}
// is_pair
template<typename T>
struct is_pair : impl::is_pair_impl<remove_cvref_t<T>> {};

template<typename T>
constexpr inline bool is_pair_v = is_pair<T>::value;
// is_smart_pointer
template<typename T>
struct is_smart_pointer : std::false_type {};

template<typename T>
struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

template<typename T, typename U>
struct is_smart_pointer<std::unique_ptr<T, U>> : std::true_type {};

template<typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

template<typename T>
struct is_smart_pointer<std::weak_ptr<T>> : std::true_type {};

template<typename T>
constexpr inline bool is_smart_pointer_v = is_smart_pointer<T>::value;
// is printable
template<typename T, typename = void>
struct is_printable : std::false_type {};

template<typename T>
struct is_printable<
    T,
    std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
    : std::true_type {};

template<typename T>
constexpr inline bool is_printable_v = is_printable<T>::value;
// are_equality_comparable
template<typename T, typename U, typename = void>
struct are_equality_comparable : std::false_type {};

template<typename T, typename U>
struct are_equality_comparable<
    T,
    U,
    std::void_t<decltype(std::declval<T>() == std::declval<U>())>>
    : std::true_type {};

template<typename T, typename U>
constexpr inline bool are_equality_comparable_v =
    are_equality_comparable<T, U>::value;

template<typename T, typename U, typename = void>
struct are_inequality_comparable : std::false_type {};

template<typename T, typename U>
struct are_inequality_comparable<
    T,
    U,
    std::void_t<decltype(std::declval<T>() != std::declval<U>())>>
    : std::true_type {};

template<typename T, typename U>
constexpr inline bool are_inequality_comparable_v =
    are_inequality_comparable<T, U>::value;
// are less comaprable
template<typename T, typename U, typename = void>
struct are_less_comparable : std::false_type {};

template<typename T, typename U>
struct are_less_comparable<
    T,
    U,
    std::void_t<decltype(std::declval<T>() < std::declval<U>())>>
    : std::true_type {};

template<typename T, typename U>
constexpr inline bool are_less_comparable_v = are_less_comparable<T, U>::value;
// are less eq comaprable
template<typename T, typename U, typename = void>
struct are_less_eq_comparable : std::false_type {};

template<typename T, typename U>
struct are_less_eq_comparable<
    T,
    U,
    std::void_t<decltype(std::declval<T>() <= std::declval<U>())>>
    : std::true_type {};

template<typename T, typename U>
constexpr inline bool are_less_eq_comparable_v =
    are_less_eq_comparable<T, U>::value;
// are greater comaprable
template<typename T, typename U, typename = void>
struct are_greater_comparable : std::false_type {};

template<typename T, typename U>
struct are_greater_comparable<
    T,
    U,
    std::void_t<decltype(std::declval<T>() > std::declval<U>())>>
    : std::true_type {};

template<typename T, typename U>
constexpr inline bool are_greater_comparable_v =
    are_greater_comparable<T, U>::value;
// are greater eq comaprable
template<typename T, typename U, typename = void>
struct are_greater_eq_comparable : std::false_type {};

template<typename T, typename U>
struct are_greater_eq_comparable<
    T,
    U,
    std::void_t<decltype(std::declval<T>() >= std::declval<U>())>>
    : std::true_type {};

template<typename T, typename U>
constexpr inline bool are_greater_eq_comparable_v =
    are_greater_eq_comparable<T, U>::value;
// make const ref
template<typename T>
struct make_const_ref
    : type_identity<
          std::add_lvalue_reference_t<std::add_const_t<remove_cvref_t<T>>>> {};

template<typename T>
using make_const_ref_t = typename make_const_ref<T>::type;
// is same as
template<typename T>
struct is_same_as {
    template<typename U>
    struct func : std::is_same<T, U> {};

    template<typename U>
    constexpr inline static bool func_v = func<U>::value;
};
// conjunction compose
template<template<typename> typename... PREDS>
struct conjunction_compose {
    template<typename T>
    using func = std::conjunction<PREDS<T>...>;

    template<typename T>
    constexpr inline static bool func_v = func<T>::value;
};
// disjunction compose
template<template<typename> typename... PREDS>
struct disjunction_compose {
    template<typename T>
    using func = std::disjunction<PREDS<T>...>;

    template<typename T>
    constexpr inline static bool func_v = func<T>::value;
};
// binary partial apply
template<template<typename, typename> typename FUNC, typename First>
struct binary_partial_apply {
    template<typename Second>
    using func = FUNC<First, Second>;

    template<typename Second>
    using func_t = typename func<Second>::type;

    template<typename Second>
    constexpr inline static decltype(func<Second>::value) func_v =
        func<Second>::value;
};
// metafunc sequential_apply
template<
    typename T,
    template<typename>
    typename FUNC,
    template<typename>
    typename... FUNC_PACK>
struct sequential_apply
    : sequential_apply<typename FUNC<T>::type, FUNC_PACK...> {};

template<typename T, template<typename> typename FUNC>
struct sequential_apply<T, FUNC> : FUNC<T> {};

template<
    typename T,
    template<typename>
    typename FUNC,
    template<typename>
    typename... FUNC_PACK>
using sequential_apply_t =
    typename sequential_apply<T, FUNC, FUNC_PACK...>::type;

template<
    typename T,
    template<typename>
    typename FUNC,
    template<typename>
    typename... FUNC_PACK>
constexpr inline auto sequential_apply_v =
    sequential_apply<T, FUNC, FUNC_PACK...>::value;
// sequential_applicator
template<template<typename> typename... FUNCS>
struct sequential_applicator {
    template<typename T>
    struct func : sequential_apply<T, FUNCS...> {};

    template<typename T>
    using func_t = typename func<T>::type;
};

}

#endif
