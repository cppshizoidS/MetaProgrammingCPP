#ifndef MATCHABLE_ANY_H
#define MATCHABLE_ANY_H

#include <any>

namespace std {
/**
 * @brief Приводит указатель на объект типа std::any к указателю на объект типа Target.
 *
 * Если содержимое объекта std::any является объектом типа Target или его производным,
 * возвращает указатель на объект типа Target. В противном случае возвращает nullptr.
 *
 * @tparam Target Целевой тип.
 * @param a Указатель на объект типа std::any.
 * @return Target* Указатель на объект типа Target или nullptr.
 */
template <typename Target>
Target const* dyn_cast(any const* a) {
  return any_cast<Target const>(a);
}
/**
 * @brief Приводит указатель на объект типа std::any к указателю на объект типа Target.
 *
 * Если содержимое объекта std::any является объектом типа Target или его производным,
 * возвращает указатель на объект типа Target. В противном случае возвращает nullptr.
 *
 * @tparam Target Целевой тип.
 * @param a Указатель на объект типа std::any.
 * @return Target* Указатель на объект типа Target или nullptr.
 */
template <typename Target>
Target* dyn_cast(any* a) {
  return any_cast<Target>(a);
}
}  // namespace std

#endif
