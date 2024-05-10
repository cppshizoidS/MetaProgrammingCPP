#ifndef SCAN_ADD_H
#define SCAN_ADD_H

#include <type_traits>
#include <utility>
#include "fold_add.h"

namespace pm {

namespace meta {

/**
 * @brief Структура для сканирования и суммирования целочисленных значений.
 *
 * Эта структура сканирует и суммирует предоставленные целочисленные значения на этапе компиляции.
 */
template <IntegralType Integral, Integral... Ns>
struct scan_add;

/**
 * @brief Псевдоним типа для удобного доступа к результату сканирования и суммирования.
 *
 * Псевдоним типа для удобного доступа к результату сканирования и суммирования целочисленных значений.
 */
template <IntegralType Integral, Integral... Ns>
using scan_add_t = typename scan_add<Integral, Ns...>::type;

/**
 * @brief Вспомогательная структура для реализации сканирования и суммирования.
 *
 * Вспомогательная структура для реализации сканирования и суммирования целочисленных значений.
 */
template <IntegralType Integral, typename Result, Integral... Ns>
struct scan_add_impl;

/**
 * @brief Частичная специализация для сканирования и суммирования одного целочисленного значения.
 */
template <IntegralType Integral, Integral N, Integral... Ms>
struct scan_add_impl<Integral, std::integer_sequence<Integral, Ms...>, N> {
  using type = std::integer_sequence<Integral, Ms..., N>;
};

/**
 * @brief Частичная специализация для сканирования и суммирования нескольких целочисленных значений.
 */
template <IntegralType Integral, Integral N1, Integral N2, Integral... Ns,
          Integral... Ms>
struct scan_add_impl<Integral, std::integer_sequence<Integral, Ms...>, N1, N2,
                     Ns...> {
  using type =
      typename scan_add_impl<Integral,
                             std::integer_sequence<Integral, Ms..., N1>,
                             N1 + N2, Ns...>::type;
};

/**
 * @brief Структура для сканирования и суммирования целочисленных значений.
 *
 * Эта структура сканирует и суммирует предоставленные целочисленные значения на этапе компиляции.
 */
template <IntegralType Integral, Integral... Ns>
struct scan_add {
  using type = typename scan_add_impl<Integral, std::integer_sequence<Integral>,
                                      Ns...>::type;
};

}  // namespace meta

}  // namespace pm

#endif
