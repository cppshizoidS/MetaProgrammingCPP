#ifndef FOLD_OR_H
#define FOLD_OR_H

#include "./fold_and.h"
#include <type_traits>

namespace pm {

namespace meta {

/**
 * @brief Структура для сворачивания логической операции ИЛИ.
 *
 * Эта структура вычисляет результат логической операции ИЛИ для предоставленных булевых значений на этапе компиляции.
 */
template <bool... bs>
struct fold_or
    : std::integral_constant<bool, not fold_and<(not bs)...>::value> {};

/**
 * @brief Переменная для сворачивания логической операции ИЛИ.
 *
 * Эта переменная предоставляет результат логической операции ИЛИ для предоставленных булевых значений как constexpr.
 */
template <bool... bs>
constexpr bool fold_or_v = fold_or<bs...>::value;

}  // namespace meta

}  // namespace pm

#endif
