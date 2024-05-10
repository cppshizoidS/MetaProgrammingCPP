#ifndef DISPATCH_H
#define DISPATCH_H

#include <cstddef>
namespace pm {

namespace meta {
/**
 * @brief Шаблонная структура для диспетчеризации на уровне компиляции.
 *
 * Данная структура предоставляет возможность диспетчеризации на уровне компиляции
 * с использованием рекурсивного шаблонного механизма.
 *
 * @tparam level Уровень диспетчеризации.
 */
template <size_t level = 0>
struct dispatch : dispatch<level - 1> {};
/**
 * @brief Специализация шаблонной структуры dispatch для остановки рекурсии.
 */
template <>
struct dispatch<0> {};

}  // namespace meta

}  // namespace pm
#endif