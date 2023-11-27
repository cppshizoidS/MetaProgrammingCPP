#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include "Meta.hpp"

namespace bits_of_q {

template <typename... ELEMS>
struct Tuple {
  constexpr Tuple() = default;
};

template <typename ELEM0, typename... ELEMS1toN>
struct Tuple<ELEM0, ELEMS1toN...> : Tuple<ELEMS1toN...> {
  template <typename T, typename... Ts>
  explicit constexpr Tuple(T &&e1, Ts &&...rest)
      : Tuple<ELEMS1toN...>(std::forward<Ts>(rest)...),
        data(std::forward<T>(e1)) {}
  ELEM0 data;
};

// template argument deduction for constructors
template <typename T, typename... Ts>
Tuple(T e1, Ts... rest)
    -> Tuple<std::unwrap_ref_decay_t<T>, std::unwrap_ref_decay_t<Ts>...>;

template <typename... ELEMS>
constexpr auto make_tuple(ELEMS &&...elems) {
  return Tuple<std::unwrap_ref_decay_t<ELEMS>...>{
      std::forward<ELEMS>(elems)...};
}

/*get*/
namespace detail {
template <size_t i, typename TUPLE>
struct get_impl : get_impl<i - 1, pop_front_t<TUPLE>> {};

template <typename TUPLE>
struct get_impl<0, TUPLE> {
  template <typename T>
  constexpr static decltype(auto) get(T &&t) {
    constexpr bool is_lvalue = std::is_lvalue_reference_v<T>;
    constexpr bool is_const = std::is_const_v<std::remove_reference_t<T>>;
    using data_t = front_t<TUPLE>;

    if constexpr (is_lvalue && is_const) {
      return static_cast<const data_t &>(static_cast<const TUPLE &>(t).data);
    }
    if constexpr (is_lvalue && !is_const) {
      return static_cast<data_t &>(static_cast<TUPLE &>(t).data);
    }
    if constexpr (!is_lvalue && is_const) {
      return static_cast<const data_t &&>(static_cast<const TUPLE &&>(t).data);
    }
    if constexpr (!is_lvalue && !is_const) {
      return static_cast<data_t &&>(static_cast<TUPLE &&>(t).data);
    }
  }
};
}  // namespace detail

template <typename TUPLE>
struct tuple_size;
template <typename... ELEMS>
struct tuple_size<Tuple<ELEMS...>>
    : std::integral_constant<size_t, sizeof...(ELEMS)> {};

template <typename TUPLE>
static constexpr size_t tuple_size_v = tuple_size<TUPLE>::value;

template <typename... Ts>
constexpr auto forward_as_tuple(Ts &&...ts) noexcept {
  return Tuple<Ts &&...>{std::forward<Ts>(ts)...};
}

namespace detail {

template <typename INDEX_SEQ>
struct make_tuple_from_fwd_tuple;

template <size_t... indices>
struct make_tuple_from_fwd_tuple<std::index_sequence<indices...>> {
  template <typename FWD_TUPLE>
  static constexpr auto f(FWD_TUPLE &&fwd) {
    return Tuple{get<indices>(std::forward<FWD_TUPLE>(fwd))...};
  }
};

template <typename FWD_INDEX_SEQ, typename TUPLE_INDEX_SEQ>
struct concat_with_fwd_tuple;

template <size_t... fwd_indices, size_t... indices>
struct concat_with_fwd_tuple<std::index_sequence<fwd_indices...>,
                             std::index_sequence<indices...>> {
  template <typename FWD_TUPLE, typename TUPLE>
  static constexpr auto f(FWD_TUPLE &&fwd, TUPLE &&t) {
    return forward_as_tuple(get<fwd_indices>(std::forward<FWD_TUPLE>(fwd))...,
                            get<indices>(std::forward<TUPLE>(t))...);
  }
};

struct tuple_cat_impl {
  template <typename FWD_TUPLE, typename TUPLE, typename... TUPLES>
  static constexpr auto f(FWD_TUPLE &&fwd, TUPLE &&t, TUPLES &&...ts) {
    return f(
        concat_with_fwd_tuple<std::make_index_sequence<
                                  tuple_size_v<std::remove_cvref_t<FWD_TUPLE>>>,
                              std::make_index_sequence<
                                  tuple_size_v<std::remove_cvref_t<TUPLE>>>>::
            f(std::forward<FWD_TUPLE>(fwd), std::forward<TUPLE>(t)),
        std::forward<TUPLES>(ts)...);
  }

  template <typename FWD_TUPLE>
  static constexpr auto f(FWD_TUPLE &&ret) {
    return make_tuple_from_fwd_tuple<std::make_index_sequence<
        tuple_size_v<FWD_TUPLE>>>::f(std::forward<FWD_TUPLE>(ret));
  }
};

}  // namespace detail

template <size_t i, typename TUPLE>
constexpr decltype(auto) get(TUPLE &&tuple) {
  return detail::get_impl<i, std::remove_cvref_t<TUPLE>>::get(
      std::forward<TUPLE>(tuple));
}

template <typename... TUPLES>
constexpr auto tuple_cat(TUPLES &&...tuples) {
  return detail::tuple_cat_impl::f(std::forward<TUPLES>(tuples)...);
}

}  // namespace bits_of_q

#endif  // TUPLE_H
