#ifndef MATCH_H
#define MATCH_H

#include <pm/meta/fold_add.h>
#include <pm/meta/fold_or.h>
#include <pm/meta/try_catch.h>

#include <boost/optional.hpp>
#include <optional>

#include "signature.h"

namespace pm {

/**
 * @brief Функция для динамического приведения типов.
 *
 */
template <typename>
void dyn_cast();

namespace detail {

/**
 * @brief Шаблон структуры, представляющей целевой тип.
 *
 * Эта структура используется для обозначения целевого типа во внутренних операциях.
 */
template <typename TargetType>
struct target_type {};

/**
 * @brief Функция для динамического приведения типов с использованием meta::try_t.
 *
 * Функция для динамического приведения типов с использованием meta::try_t.
 */
template <typename TargetType, typename SourceType>
[[maybe_unused]] constexpr auto matcher_dyn_cast(meta::try_t, target_type<TargetType>,
                                                 SourceType* p) -> decltype(dyn_cast<TargetType>(p)) {
  return dyn_cast<TargetType>(p);
}

/**
 * @brief Функция для динамического приведения типов с использованием meta::catch_t.
 *
 * Функция для динамического приведения типов с использованием meta::catch_t.
 */
template <typename TargetType, typename SourceType>
constexpr auto matcher_dyn_cast(meta::catch_t, target_type<TargetType>,
                                SourceType* p) -> decltype(dynamic_cast<TargetType*>(p)) {
  return dynamic_cast<TargetType*>(p);
}

/**
 * @brief Структура для хранения информации о результатах функций.
 *
 * Эта структура хранит информацию о результатах функций и типе результата.
 */
template <typename Function, typename... Functions>
struct result {
  using type = typename pm::signature<Function>::result_type;

  template <typename...>
  struct type_list {};

  template <typename F>
  using as_type = std::decay_t<type>;

  using all_results =
      type_list<typename pm::signature<Functions>::result_type...>;
  using expected_results = type_list<as_type<Functions>...>;

  static_assert(std::is_same<all_results, expected_results>::value,
                "All functions must have the same result type.");
};

/**
 * @brief Структура для хранения информации о типе результата функций.
 *
 * Эта структура хранит информацию о типе результата функций и используется при матчинге.
 */
template <typename... Functions>
struct result_type_info {
  static constexpr size_t sum_of_arguments =
      meta::fold_add_v<size_t, signature<Functions>::number_of_arguments...>;
  using wrapped_result_t = typename result<Functions...>::type;
  using result_t = std::conditional_t<
      std::is_same<void, wrapped_result_t>::value, bool,
      std::conditional_t<sum_of_arguments == size_t(sizeof...(Functions)),
                         std::optional<wrapped_result_t>, wrapped_result_t> >;
};

/**
 * @brief Структура для вызова функций матчинга.
 *
 * Эта структура предназначена для вызова функций матчинга с учетом их результатов.
 */
template <typename ResultType>
struct invoker {
  template <typename Function>
  static constexpr decltype(auto) apply(Function& f) {
    return f();
  }

  template <typename Function, typename Arg>
  static constexpr decltype(auto) apply(Function& f, Arg& a) {
    return f(a);
  }
};

template <>
struct invoker<void> {
  template <typename Function>
  static constexpr decltype(auto) apply(Function& f) {
    return f(), true;
  }

  template <typename Function, typename Arg>
  static constexpr decltype(auto) apply(Function& f, Arg& a) {
    return f(a), true;
  }
};

/**
 * @brief Вспомогательная функция для реализации матчинга.
 *
 * Вспомогательная функция, используемая для реализации матчинга.
 */
template <typename ResultTypeInfo, typename Type, typename Lambda,
          typename... Lambdas>
constexpr auto matcher_impl(Type* x, Lambda& l, Lambdas&... ls)
    -> std::enable_if_t<signature<Lambda>::number_of_arguments == 1,
                        typename ResultTypeInfo::result_t> {
  if (auto result = l(*x); result.has_value())
    return result.value();
  else
    return matcher_impl<ResultTypeInfo>(x, ls...);
}

template <typename ResultTypeInfo, typename Type>
constexpr auto matcher_impl(Type*) -> typename ResultTypeInfo::result_t {
  return std::nullopt;
}

template <typename ResultTypeInfo, typename Type, typename Lambda1,
          typename Lambda2, typename... Lambdas>
constexpr auto matcher_impl(Type* x, Lambda1& l1, Lambda2& l2, Lambdas&... ls)
    -> std::enable_if_t<signature<Lambda1>::number_of_arguments == 0,
                        typename ResultTypeInfo::result_t> {
  if (auto result = l1(); result.has_value())
    return result.value();
  else
    return matcher_impl<ResultTypeInfo>(x, l2, ls...);
}

template <typename ResultTypeInfo, typename Type, typename Lambda,
          typename... Lambdas>
constexpr auto matcher_impl(Type*, Lambda& l)
    -> std::enable_if_t<signature<Lambda>::number_of_arguments == 0,
                        typename ResultTypeInfo::result_t> {
  if (auto result = l(); result.has_value())
    return result.value();
  else
    return std::nullopt;
}

/**
 * @brief Функция для установки значения по умолчанию.
 *
 * Эта функция используется для установки значения по умолчанию в матчинге.
 */
template <typename Value>
constexpr auto otherwise(Value value) {
  return [value = std::move(value)] { return value; };
}

/**
 * @brief Функция для создания матчера.
 *
 * Эта функция создает матчер на основе предоставленных лямбда-функций.
 */
template <typename... Lambdas>
constexpr auto matcher(Lambdas&&... lambdas) {
  static_assert(
      meta::fold_and_v<((signature<Lambdas>::number_of_arguments == 0) or
                        (signature<Lambdas>::number_of_arguments == 1))...>,
      "Can only match on lambdas with one argument.");

  using result_info_t = ::pm::detail::result_type_info<Lambdas...>;

  constexpr auto num_args = result_info_t::sum_of_arguments;
  static_assert(
      num_args == sizeof...(Lambdas) or num_args == sizeof...(Lambdas) - 1,
      "There can be only one default value defined per matcher.");

  return [=](auto& x) -> typename result_info_t::result_t {
    return ::pm::detail::matcher_impl<result_info_t>(&x, lambdas...);
  };
}

/**
 * @brief Функция для создания матчера.
 *
 * Эта функция создает матчер для переданного значения.
 */
template <typename Type>
auto match(Type&& x) {
  return [x = std::forward<Type>(x)](auto&&... lambdas) mutable -> decltype(auto) {
    return matcher(std::forward<decltype(lambdas)>(lambdas)...)(x);
  };
}

/**
 * @brief Функция для создания матчера.
 *
 * Эта функция создает матчер для переданного значения.
 */
template <typename Type>
auto match(Type& x) {
  return [&x](auto&&... lambdas) mutable -> decltype(auto) {
    return matcher(std::forward<decltype(lambdas)>(lambdas)...)(x);
  };
}

}  // namespace pm

#endif
