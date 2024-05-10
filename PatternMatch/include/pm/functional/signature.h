#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <boost/config.hpp>
#include <tuple>
#include <type_traits>

namespace pm {

/**
 * @brief Метафункция для извлечения сигнатуры вызываемого объекта.
 *
 * Эта метафункция специализируется для различных типов вызываемых объектов,
 * включая указатели на функции, указатели на методы и функторы.
 *
 * @tparam C Тип вызываемого объекта.
 */
template <class C>
struct signature : signature<decltype(&std::decay_t<C>::operator())> {};

// Специализация для указателей на методы
template <class C, typename Result, typename... Args>
struct signature<Result (C::*)(Args...)> : signature<Result(Args...)> {};

// Специализация для const указателей на методы
template <class C, typename Result, typename... Args>
struct signature<Result (C::*)(Args...) const> : signature<Result(Args...)> {};

// Специализация для volatile указателей на методы
template <class C, typename Result, typename... Args>
struct signature<Result (C::*)(Args...) volatile> : signature<Result(Args...)> {
};

// Специализация для const volatile указателей на методы
template <class C, typename Result, typename... Args>
struct signature<Result (C::*)(Args...) const volatile>
    : signature<Result(Args...)> {};

// Специализация для указателей на функции
template <typename Result, typename... Args>
struct signature<Result (*)(Args...)> : signature<Result(Args...)> {};

// Специализация для ссылок на функции
template <typename Result, typename... Args>
struct signature<Result (&)(Args...)> : signature<Result(Args...)> {};

/**
 * @brief Представляет сигнатуру функции.
 *
 * Эта структура предоставляет информацию на этапе компиляции о типах и типе
 * результата функции.
 *
 * @tparam Result Тип результата функции.
 * @tparam Args   Типы аргументов функции.
 */
template <typename Result, typename... Args>
struct signature<Result(Args...)> {
  /// Количество аргументов функции.
  static constexpr auto num_args = sizeof...(Args);

  /// Тип сигнатуры функции.
  typedef Result type(Args...);

  /// Тип результата функции.
  using result_type = Result;

  /// Количество аргументов функции.
  static constexpr size_t number_of_arguments = sizeof...(Args);

  /**
   * @brief Представляет тип n-го аргумента функции.
   *
   * @tparam n Индекс аргумента.
   */
  template <size_t n>
  using argument = typename std::tuple_element<n, std::tuple<Args...>>;
};

/**
 * @brief Концепция для проверки, можно ли вызвать объект с указанными
 * аргументами.
 *
 * Эта концепция проверяет, можно ли вызвать объект с указанными аргументами
 * и вернуть результат ожидаемого типа.
 *
 * @tparam Callable Тип вызываемого объекта.
 */
template <typename Callable>
concept Invocable = requires(Callable&& f) {
  {
    std::forward<Callable>(f)(
        std::declval<
            typename signature<std::decay_t<Callable>>::template argument<0>>())
  } -> std::same_as<typename signature<std::decay_t<Callable>>::result_type>;
};

/**
 * @brief Концепция для проверки, можно ли вызвать объект без генерации
 * исключений.
 *
 * Эта концепция проверяет, можно ли вызвать объект с указанными аргументами
 * без генерации исключений и вернуть результат ожидаемого типа.
 *
 * @tparam Callable Тип вызываемого объекта.
 */
template <typename Callable>
concept NothrowInvocable = requires(Callable&& f) {
  {
    std::forward<Callable>(f)(
        std::declval<
            typename signature<std::decay_t<Callable>>::template argument<0>>())
  } noexcept
      -> std::same_as<typename signature<std::decay_t<Callable>>::result_type>;
};

/**
 * @brief Псевдоним для типа сигнатуры вызываемого объекта.
 *
 * Этот псевдоним обеспечивает простой способ получить тип сигнатуры вызываемого
 * объекта.
 *
 * @tparam Callable Тип вызываемого объекта.
 */
template <typename Callable>
using signature_t = typename signature<std::decay_t<Callable>>::type;

/**
 * @brief Псевдоним для типа результата вызываемого объекта.
 *
 * Этот псевдоним обеспечивает простой способ получить тип результата
 * вызываемого объекта.
 *
 * @tparam Callable Тип вызываемого объекта.
 */
template <typename Callable>
using result_t = typename signature<std::decay_t<Callable>>::result_type;

}  // namespace pm

#endif
