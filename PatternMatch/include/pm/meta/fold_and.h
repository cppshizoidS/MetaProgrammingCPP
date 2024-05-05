#ifndef FOLD_AND_H
#define FOLD_AND_H

#include <type_traits>

#include "bools.h"

namespace pm {

namespace meta {

namespace detail {

/**
 * @brief Шаблон структуры, преобразующей значение в true.
 *
 * Эта структура используется во внутренних операциях для преобразования значения в true.
 */
template <bool b>
struct as_true {
  static constexpr bool value = true;
};

}  // namespace detail

/**
 * @brief Структура для сворачивания логической операции И.
 *
 * Эта структура вычисляет результат логической операции И для предоставленных булевых значений на этапе компиляции.
 */
template <bool... bs>
struct fold_and
    : std::integral_constant<bool,
                             std::is_same<bools<detail::as_true<bs>::value...>,
                                          bools<bs...> >::value> {};

/**
 * @brief Переменная для сворачивания логической операции И.
 *
 * Эта переменная предоставляет результат логической операции И для предоставленных булевых значений как constexpr.
 */
template <bool... bs>
constexpr bool fold_and_v = fold_and<bs...>::value;

}  // namespace meta

}  // namespace pm

#endif
