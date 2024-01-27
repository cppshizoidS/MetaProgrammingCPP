#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <type_traits>
#include <utility>

namespace intersection {

template <typename Xs, typename Ys>
struct intersection_t;

namespace detail {

template <typename T, typename U>
struct intersection_impl;

template <template <typename...> class Xs, typename... XArgs, typename... YArgs>
struct intersection_impl<Xs<XArgs...>, Xs<YArgs...>> {
    template <typename... Args>
    struct filter_types;

    template <typename... Args>
    using type = typename filter_types<Args...>::type;

    template <typename... Args>
    struct filter_types {
        using type = Xs<Args...>;
    };

    template <typename Head, typename... Tail>
    struct filter_types<Head, Tail...> {
        using type = typename std::conditional_t<(std::is_same<Head, YArgs>::value || ...),
                                                 filter_types<Tail...>,
                                                 filter_types<Head, Tail...>>::type;
    };
};

} // namespace detail

template <typename Xs, typename Ys>
using intersection = typename detail::intersection_impl<Xs, Ys>::template type<>;

} // namespace intersection

#endif // INTERSECTION_HPP
