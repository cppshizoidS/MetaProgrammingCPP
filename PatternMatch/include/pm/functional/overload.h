#ifndef OVERLOAD_H
#define OVERLOAD_H

#include <type_traits>
#include <utility>

namespace pm {
/**
 * @brief Класс overloaded предоставляет механизм для создания объектов, перегружающих оператор ().
 *
 * @tparam Lambdas Параметры шаблона, которые должны быть лямбда-выражениями или функциональными объектами.
 */
template <typename... Lambdas>
struct overloaded {};

/**
 * @brief Специализация класса overloaded для нескольких параметров шаблона.
 *
 * @tparam Lambda1 Первое лямбда-выражение или функциональный объект.
 * @tparam Lambdas Остальные лямбда-выражения или функциональные объекты.
 */
template <typename Lambda1, typename... Lambdas>
struct overloaded<Lambda1, Lambdas...> : public Lambda1,
                                         public overloaded<Lambdas...> {
  using Lambda1::operator();
  using overloaded<Lambdas...>::operator();

  /**
   * @brief Конструктор, инициализирующий объект overloaded с переданными лямбда-выражениями или функциональными объектами.
   *
   * @tparam UniLambda1 Тип первого переданного лямбда-выражения или функционального объекта.
   * @tparam UniLambdas Типы остальных переданных лямбда-выражений или функциональных объектов.
   * @param l1 Первое лямбда-выражение или функциональный объект.
   * @param lambdas Остальные лямбда-выражения или функциональные объекты.
   */
  template <typename UniLambda1, typename... UniLambdas>
  overloaded(UniLambda1&& l1, UniLambdas&&... lambdas)
      : Lambda1{std::forward<UniLambda1>(l1)},
        overloaded<Lambdas...>{std::forward<UniLambdas>(lambdas)...} {}
};

/**
 * @brief Специализация класса overloaded для единственного параметра шаблона.
 *
 * @tparam Lambda1 Единственное лямбда-выражение или функциональный объект.
 */
template <typename Lambda1>
struct overloaded<Lambda1> : public Lambda1 {
  using Lambda1::operator();

  /**
   * @brief Конструктор, инициализирующий объект overloaded с переданным лямбда-выражением или функциональным объектом.
   *
   * @tparam UniLambda1 Тип переданного лямбда-выражения или функционального объекта.
   * @param l1 Лямбда-выражение или функциональный объект.
   */
  template <typename UniLambda1, typename = std::enable_if_t<std::is_same<
                                     Lambda1, std::decay_t<UniLambda1>>::value>>
  overloaded(UniLambda1&& l1) : Lambda1{std::forward<UniLambda1>(l1)} {}
};

namespace detail {
/**
 * @brief Шаблонная структура free_function_wrapper оборачивает свободные функции в лямбда-выражения.
 */
template <typename Function>
struct free_function_wrapper {
  /**
   * @brief Применяет переданную свободную функцию.
   *
   * @tparam F Тип переданной функции.
   * @param f Свободная функция.
   * @return decltype(auto) Возвращаемое значение функции.
   */
  template <typename F>
  static decltype(auto) apply(F&& f) {
    return std::forward<F>(f);
  }
};

/**
 * @brief Частичная специализация free_function_wrapper для указателей на свободные функции.
 */
template <typename R, typename... Args>
struct free_function_wrapper<R (*)(Args...)> {
  /**
   * @brief Применяет переданный указатель на свободную функцию.
   *
   * @tparam F Тип переданного указателя на свободную функцию.
   * @param f Указатель на свободную функцию.
   * @return auto Возвращаемое значение функции.
   */
  template <typename F>
  static auto apply(F* f) {
    return [f](Args... args) { return f(std::forward<Args>(args)...); };
  }
};

/**
 * @brief Оборачивает переданную функцию в лямбда-выражение.
 *
 * @tparam Function Тип переданной функции.
 * @param f Функция или указатель на свободную функцию.
 * @return decltype(auto) Обёрнутая функция.
 */
template <typename Function>
decltype(auto) wrap_free_function(Function&& f) {
  return free_function_wrapper<std::decay_t<Function>>::apply(
      std::forward<Function>(f));
}

/**
 * @brief Создаёт объект overloaded с переданными лямбда-выражениями или функциональными объектами.
 *
 * @tparam Lambdas Типы переданных лямбда-выражений или функциональных объектов.
 * @param lambdas Лямбда-выражения или функциональные объекты.
 * @return auto Созданный объект overloaded.
 */
template <typename... Lambdas>
auto overload(Lambdas&&... lambdas) -> overloaded<std::decay_t<Lambdas>...> {
  return {wrap_free_function(std::forward<Lambdas>(lambdas))...};
}

}  // namespace detail

/**
 * @brief Создаёт объект overloaded с переданными лямбда-выражениями или функциональными объектами.
 *
 * @tparam Lambdas Типы переданных лямбда-выражений или функциональных объектов.
 * @param lambdas Лямбда-выражения или функциональные объекты.
 * @return decltype(auto) Созданный объект overloaded.
 */
template <typename... Lambdas>
decltype(auto) overload(Lambdas&&... lambdas) {
  return detail::overload(
      detail::wrap_free_function(std::forward<Lambdas>(lambdas))...);
}
}  // namespace pm

#endif
