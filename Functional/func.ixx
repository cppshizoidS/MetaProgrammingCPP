export module func;

import std;

namespace meta {
/**
 * @brief Проекция для функции, принимающей проекцию элементов.
 *
 * @tparam Fn Тип функции, принимающей проекцию элементов.
 * @tparam Proj Тип проекции элементов.
 */
export template <typename Fn, typename Proj = std::identity>
struct proj {
    Fn fn;
    Proj prj{};

    template <typename... Args>
    constexpr auto operator()(Args&&... args)
        noexcept(noexcept(std::invoke(fn, std::invoke(prj, static_cast<decltype(args)&&>(args))...)))
        -> decltype(std::invoke(fn, std::invoke(prj, static_cast<decltype(args)&&>(args))...))
    {
        return std::invoke(fn, std::invoke(prj, static_cast<decltype(args)&&>(args))...);
    }

    template <typename... Args>
    constexpr auto operator()(Args&&... args) const
        noexcept(noexcept(std::invoke(fn, std::invoke(prj, static_cast<decltype(args)&&>(args))...)))
        -> decltype(std::invoke(fn, std::invoke(prj, static_cast<decltype(args)&&>(args))...))
    {
        return std::invoke(fn, std::invoke(prj, static_cast<decltype(args)&&>(args))...);
    }
};
/**
 * @brief Конструктор шаблона `proj`.
 *
 * @tparam F Тип функции.
 * @tparam P Тип проекции.
 * @param fn Функция.
 * @param prj Проекция.
 */
template <typename F, typename P = std::identity>
proj(F, P = {}) -> proj<F, P>;
/**
 * @brief Проекция для функции, принимающей два аргумента и их проекции.
 *
 * @tparam Fn Тип функции, принимающей два аргумента и их проекции.
 * @tparam Lhs Тип левой проекции.
 * @tparam Rhs Тип правой проекции.
 */
export template <typename Fn, typename Lhs = std::identity, typename Rhs = std::identity>
struct proj2 {
    Fn fn;
    Lhs lhs{};
    Rhs rhs{};

    template <typename Arg1, typename Arg2>
    constexpr auto operator()(Arg1&& arg1, Arg2&& arg2) const
        noexcept(noexcept(std::invoke(fn, std::invoke(lhs, static_cast<decltype(arg1)&&>(arg1)),
                                      std::invoke(rhs, static_cast<decltype(arg2)&&>(arg2)))))
            -> decltype(std::invoke(fn, std::invoke(lhs, static_cast<decltype(arg1)&&>(arg1)),
                                    std::invoke(rhs, static_cast<decltype(arg2)&&>(arg2))))
    {
        return std::invoke(fn, std::invoke(lhs, static_cast<decltype(arg1)&&>(arg1)),
                           std::invoke(rhs, static_cast<decltype(arg2)&&>(arg2)));
    }
};
/**
 * @brief Конструктор шаблона `proj2`.
 *
 * @tparam F Тип функции.
 * @tparam L Тип левой проекции.
 * @tparam R Тип правой проекции.
 * @param fn Функция.
 * @param lhs Левая проекция.
 * @param rhs Правая проекция.
 */
template <typename F, typename L = std::identity, typename R = std::identity>
proj2(F, L = {}, R = {}) -> proj2<F, L, R>;

namespace detail {
/**
 * @brief Функтор для ленивого применения функции к кортежу.
 *
 * @tparam Func Тип функции.
 */
template <typename Func>
struct lazy_apply {
    Func func_;

    template <typename Tuple>
    constexpr auto operator()(Tuple&& tuple) &
        noexcept(noexcept(std::apply(func_, static_cast<decltype(tuple)&&>(tuple))))
        -> decltype(std::apply(func_, static_cast<decltype(tuple)&&>(tuple)))
    {
        return std::apply(func_, static_cast<decltype(tuple)&&>(tuple));
    }

    template <typename Tuple>
    constexpr auto operator()(Tuple&& tuple) const&
        noexcept(noexcept(std::apply(func_, static_cast<decltype(tuple)&&>(tuple))))
        -> decltype(std::apply(func_, static_cast<decltype(tuple)&&>(tuple)))
    {
        return std::apply(func_, static_cast<decltype(tuple)&&>(tuple));
    }

    template <typename Tuple>
    constexpr auto operator()(Tuple&& tuple) &&
        noexcept(noexcept(std::apply(std::move(func_), static_cast<decltype(tuple)&&>(tuple))))
        -> decltype(std::apply(std::move(func_), static_cast<decltype(tuple)&&>(tuple)))
    {
        return std::apply(std::move(func_), static_cast<decltype(tuple)&&>(tuple));
    }

    template <typename Tuple>
    constexpr auto operator()(Tuple&& tuple) const&&
        noexcept(noexcept(std::apply(std::move(func_), static_cast<decltype(tuple)&&>(tuple))))
        -> decltype(std::apply(std::move(func_), static_cast<decltype(tuple)&&>(tuple)))
    {
        return std::apply(std::move(func_), static_cast<decltype(tuple)&&>(tuple));
    }
};
/**
 * @brief Функтор для распаковки функции.
 */
struct unpack_fn {
    /**
     * @brief Оператор вызова для создания объекта `lazy_apply`.
     *
     * @tparam Func Тип функции.
     * @param func Функция.
     * @return lazy_apply<std::decay_t<Func>> Объект для ленивого применения функции.
     */
    template <typename Func>
    constexpr auto operator()(Func&& func) const
        -> lazy_apply<std::decay_t<Func>>
    {
        return lazy_apply<std::decay_t<Func>>{.func_ = static_cast<decltype(func)&&>(func)};
    }
};

} // namespace detail
/**
 * @brief Объект для распаковки функций.
 */
export inline constexpr auto unpack = detail::unpack_fn{};

namespace pred {

namespace detail {

template <typename Lambda>
struct predicate : Lambda {};

template <typename L>
predicate(L) -> predicate<L>;

template <typename Op>
inline constexpr auto cmp = [](auto&& val) {
    return predicate{[val = static_cast<decltype(val)&&>(val)](auto const& other) {
            return Op{}(other, val);
        }};
};

} // namespace detail
/**
 * @brief Функция отрицания предиката.
 *
 * @tparam P Тип предиката.
 * @param pred Предикат для отрицания.
 * @return detail::predicate Отрицание предиката.
 */
 export inline constexpr auto not_ = [](auto&& pred) {
    return detail::predicate([p = static_cast<decltype(pred)&&>(pred)] (auto const&... args) {
        return !std::invoke(p, static_cast<decltype(args)&&>(args)...);
    });
};
 /**
 * @brief Комбинированный предикат "и".
 *
 * @param p Первый предикат.
 * @param and_ Второй предикат.
 * @return detail::predicate Комбинированный предикат "и".
 */
export inline constexpr auto both = [](auto&& p, auto&& and_) {
    return detail::predicate{[p1 = static_cast<decltype(p)&&>(p), p2 = static_cast<decltype(and_)&&>(and_)] (auto const&... args) {
        return std::invoke(p1, args...) && std::invoke(p2, args...);
    }};
};
/**
 * @brief Комбинированный предикат "или".
 *
 * @param p Первый предикат.
 * @param or_ Второй предикат.
 * @return detail::predicate Комбинированный предикат "или".
 */
export inline constexpr auto either = [](auto&& p, auto&& or_) {
     return detail::predicate{[p1 = static_cast<decltype(p)&&>(p), p2 = static_cast<decltype(or_)&&>(or_)] (auto const&... args) {
        return std::invoke(p1, args...) || std::invoke(p2, args...);
     }};
};

namespace detail {

export template <typename P>
constexpr auto operator!(detail::predicate<P> pred)
{
    return not_(std::move(pred));
}

export template <typename L, typename R>
constexpr auto operator&&(detail::predicate<L> lhs, detail::predicate<R> rhs)
{
    return both(std::move(lhs), std::move(rhs));
}

export template <typename L, typename R>
constexpr auto operator||(detail::predicate<L> lhs, detail::predicate<R> rhs)
{
    return either(std::move(lhs), std::move(rhs));
}

} // namespace detail
/**
 * @brief Комбинированный предикат "ни то, ни другое".
 *
 * @param p1 Первый предикат.
 * @param nor Второй предикат.
 * @return detail::predicate Комбинированный предикат "ни то, ни другое".
 */
 export inline constexpr auto neither = [](auto&& p1, auto&& nor) {
    return not_(either(static_cast<decltype(p1)&&>(p1), static_cast<decltype(nor)&&>(nor)));
};
 /**
 * @brief Предикаты равенства/неравенства.
 * eq:
 * @param val Значение, с которым сравнивается.
 * @return detail::predicate Предикат равенства.
 * neq:
 * @param val Значение, с которым сравнивается.
 * @return detail::predicate Предикат неравенства.
 * lt:
 * @param val Значение, с которым сравнивается.
 * @return detail::predicate Предикат "меньше чем".
 * gt:
 * @param val Значение, с которым сравнивается.
 * @return detail::predicate Предикат "больше чем".
 * leq:
 * @param val Значение, с которым сравнивается.
 * @return detail::predicate Предикат "меньше или равно".
 * geq:
 * @param val Значение, с которым сравнивается.
 * @return detail::predicate Предикат "больше или равно".
 */
 export inline constexpr auto eq = detail::cmp<std::ranges::equal_to>;
 export inline constexpr auto neq = detail::cmp<std::ranges::not_equal_to>;
 export inline constexpr auto lt = detail::cmp<std::ranges::less>;
 export inline constexpr auto gt = detail::cmp<std::ranges::greater>;
 export inline constexpr auto leq = detail::cmp<std::ranges::less_equal>;
 export inline constexpr auto geq = detail::cmp<std::ranges::greater_equal>;
 /**
 * @brief Предикат, всегда возвращающий `true`.
 *
 * @return detail::predicate Предикат, всегда возвращающий `true`.
 */
   export inline constexpr auto true_ = detail::predicate{[](auto const&...) -> bool { return true; }};
/**
 * @brief Предикат, всегда возвращающий `false`.
 *
 * @return detail::predicate Предикат, всегда возвращающий `false`.
 */
   export inline constexpr auto false_ = detail::predicate{[](auto const&...) -> bool { return false; }};
/**
 * @brief Предикат, возвращающий исходное значение типа `bool`.
 *
 * @return detail::predicate Предикат, возвращающий исходное значение типа `bool`.
 */
   export inline constexpr auto id = detail::predicate{[](bool b) -> bool { return b; }};
/**
 * @brief Предикат для положительных чисел.
 *
 * @param val Значение, проверяемое на положительность.
 * @return detail::predicate Предикат для положительных чисел.
 */
   export inline constexpr auto positive = detail::predicate{[](auto const& val) -> bool {
    return val > decltype(val){0};
}};
/**
 * @brief Предикат для отрицательных чисел.
 *
 * @param val Значение, проверяемое на отрицательность.
 * @return detail::predicate Предикат для отрицательных чисел.
 */
   export inline constexpr auto negative = detail::predicate{[](auto const& val) -> bool {
    return val < decltype(val){0};
}};
/**
 * @brief Предикат для ненулевых чисел.
 *
 * @param val Значение, проверяемое на ненулевость.
 * @return detail::predicate Предикат для ненулевых чисел.
 */
   export inline constexpr auto nonzero = detail::predicate{[](auto const& val) -> bool {
    return val != decltype(val){0};
}};
/**
 * @brief Предикат, проверяющий принадлежность к списку значений.
 *
 * @param vals Список значений.
 * @return detail::predicate Предикат, проверяющий принадлежность к списку значений.
 */
    export inline constexpr auto in = [](auto const&... vals)  requires (sizeof...(vals) > 0)
{
    return detail::predicate{[vals...](auto const& arg) -> bool {
        return ((arg == vals) || ...);
    }};
};
/**
 * @brief Предикат для чётных чисел.
 *
 * @param val Значение, проверяемое на чётность.
 * @return detail::predicate Предикат для чётных чисел.
 */
  export inline constexpr auto even = detail::predicate([](auto const& val) -> bool {
    return val % decltype(val){2} == decltype(val){0};
});
/**
 * @brief Предикат для нечётных чисел.
 *
 * @param val Значение, проверяемое на нечётность.
 * @return detail::predicate Предикат для нечётных чисел.
 */
  export inline constexpr auto odd = detail::predicate([](auto const& val) -> bool {
  return val % decltype(val){2} != decltype(val){0};
});

} // namespace pred

namespace cmp {

namespace detail {
/**
 * @brief Функция для нахождения минимального значения среди двух элементов.
 */
struct min_fn {
    /**
     * @brief Оператор вызова для нахождения минимального значения.
     *
     * @tparam T Тип первого элемента.
     * @tparam U Тип второго элемента.
     * @tparam Cmp Тип компаратора.
     * @param t Первый элемент.
     * @param u Второй элемент.
     * @param cmp Компаратор (по умолчанию std::ranges::less).
     * @return std::common_reference_t<T, U> Минимальное значение.
     */
    template <typename T, typename U, typename Cmp = std::ranges::less>
        requires std::strict_weak_order<Cmp&, T&, U&>
    [[nodiscard]]
    constexpr auto operator()(T&& t, U&& u, Cmp cmp = Cmp{}) const
        -> std::common_reference_t<T, U>
    {
        return std::invoke(cmp, u, t) ? static_cast<decltype(u)&&>(u) : static_cast<decltype(t)&&>(t);
    }
};
/**
 * @brief Функция для нахождения максимального значения среди двух элементов.
 */
struct max_fn {
    /**
     * @brief Оператор вызова для нахождения максимального значения.
     *
     * @tparam T Тип первого элемента.
     * @tparam U Тип второго элемента.
     * @tparam Cmp Тип компаратора.
     * @param t Первый элемент.
     * @param u Второй элемент.
     * @param cmp Компаратор (по умолчанию std::ranges::less).
     * @return std::common_reference_t<T, U> Максимальное значение.
     */
    template <typename T, typename U, typename Cmp = std::ranges::less>
        requires std::strict_weak_order<Cmp&, T&, U&>
    [[nodiscard]]
    constexpr auto operator()(T&& t, U&& u, Cmp cmp = Cmp{}) const
        -> std::common_reference_t<T, U>
    {
        return !std::invoke(cmp, u, t) ? static_cast<decltype(u)&&>(u) : static_cast<decltype(t)&&>(t);
    }
};

} // namespace detail

  export inline constexpr auto min = detail::min_fn{};
  export inline constexpr auto max = detail::max_fn{};

} // namespace cmp

} // namespace meta
