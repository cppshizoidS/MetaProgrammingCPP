#ifndef FOLD_ADD_H
#define FOLD_ADD_H

#include <type_traits>

namespace pm::meta {

/**
 * @brief Концепция для целочисленных типов.
 *
 * Эта концепция проверяет, является ли данный тип целочисленным.
 */
template <typename T>
concept IntegralType = std::is_integral_v<T>;

/**
 * @brief Структура для сворачивания сложения целочисленных значений.
 *
 * Эта структура вычисляет сумму предоставленных целочисленных значений на этапе компиляции.
 */
template <IntegralType Integral, Integral... ns>
struct fold_add;

/**
 * @brief Частичная специализация для случая, когда есть только одно целочисленное значение.
 */
template <IntegralType Integral, Integral n>
struct fold_add<Integral, n> : std::integral_constant<Integral, n> {};

/**
 * @brief Частичная специализация для сворачивания сложения нескольких целочисленных значений.
 */
template <IntegralType Integral, Integral n, Integral... ns>
struct fold_add<Integral, n, ns...>
    : std::integral_constant<Integral, n + fold_add<Integral, ns...>::value> {};

/**
 * @brief Шаблон переменной для сворачивания сложения целочисленных значений.
 *
 * Этот шаблон переменной предоставляет сумму предоставленных целочисленных значений как constexpr.
 */
template <IntegralType Integral, Integral... ns>
constexpr Integral fold_add_v = fold_add<Integral, ns...>::value;

} // namespace pm::meta

#endif
