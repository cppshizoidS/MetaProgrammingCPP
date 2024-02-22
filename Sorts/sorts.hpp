#ifndef METASORT_HPP
#define METASORT_HPP

#include <array>
#include <cstddef>

namespace metasort {

template <bool Condition, typename TrueType, typename FalseType>
struct conditional;

template <typename TrueType, typename FalseType>
struct conditional<true, TrueType, FalseType> {
    using type = TrueType;
};

template <typename TrueType, typename FalseType>
struct conditional<false, TrueType, FalseType> {
    using type = FalseType;
};

#if __cplusplus >= 201402L
template <bool Condition, typename TrueType, typename FalseType>
using conditional_t =
    typename conditional<Condition, TrueType, FalseType>::type;
#endif

template <typename T, T... Values>
struct sequence {
    using value_type = T;

    static constexpr std::size_t size() { return sizeof...(Values); }

    static constexpr std::array<T, sizeof...(Values)> to_array() {
        return std::array<T, sizeof...(Values)>{Values...};
    }
};

template <typename SequenceT, std::size_t Index>
struct get;

template <typename T, std::size_t Index>
struct get<sequence<T>, Index> {};

template <typename T, T First, T... Rest>
struct get<sequence<T, First, Rest...>, 0> {
    static constexpr T value = First;
};

template <typename T, T First, T... Rest, std::size_t Index>
struct get<sequence<T, First, Rest...>, Index> {
    static_assert(Index - 1 < sizeof...(Rest), "sequence index out of bounds");
    static constexpr T value = get<sequence<T, Rest...>, Index - 1>::value;
};

#if __cplusplus >= 201703L
template <typename SequenceT, std::size_t Index>
inline constexpr typename SequenceT::value_type get_v =
    get<SequenceT, Index>::value;
#endif

template <typename... SequenceTs>
struct sequence_cat;

template <typename T, T... Values>
struct sequence_cat<sequence<T, Values...>> {
    using type = sequence<T, Values...>;
};

template <typename T, T... ValuesFirst, T... ValuesSecond>
struct sequence_cat<sequence<T, ValuesFirst...>, sequence<T, ValuesSecond...>> {
    using type = sequence<T, ValuesFirst..., ValuesSecond...>;
};

template <typename SequenceTFirst, typename SequenceTSecond,
          typename... SequenceTRest>
struct sequence_cat<SequenceTFirst, SequenceTSecond, SequenceTRest...> {
    using type = typename sequence_cat<
        typename sequence_cat<SequenceTFirst, SequenceTSecond>::type,
        SequenceTRest...>::type;
};

#if __cplusplus >= 201402L
template <typename... SequenceTs>
using sequence_cat_t = typename sequence_cat<SequenceTs...>::type;
#endif

template <typename SequenceT, std::size_t N>
struct first_n;

template <typename T>
struct first_n<sequence<T>, 0> {
    using type = sequence<T>;
};

template <typename T, T First, T... Rest>
struct first_n<sequence<T, First, Rest...>, 0> {
    using type = sequence<T>;
};

template <typename T, T First, T... Rest>
struct first_n<sequence<T, First, Rest...>, 1> {
    using type = sequence<T, First>;
};

template <typename T, T First, T... Rest, std::size_t N>
struct first_n<sequence<T, First, Rest...>, N> {
    static_assert(N - 1 <= sizeof...(Rest), "sequence index out of bounds");
    using type = typename sequence_cat<
        typename first_n<sequence<T, Rest...>, N - 1>::type,
        sequence<T, First>>::type;
};

#if __cplusplus >= 201402L
template <typename SequenceT, std::size_t N>
using first_n_t = typename first_n<SequenceT, N>::type;
#endif

template <typename SequenceT, std::size_t N>
struct remove_first_n;

template <typename T>
struct remove_first_n<sequence<T>, 0> {
    using type = sequence<T>;
};

template <typename T, T First, T... Rest>
struct remove_first_n<sequence<T, First, Rest...>, 0> {
    using type = sequence<T, First, Rest...>;
};

template <typename T, T First, T... Rest>
struct remove_first_n<sequence<T, First, Rest...>, 1> {
    using type = sequence<T, Rest...>;
};

template <typename T, T First, T... Rest, std::size_t N>
struct remove_first_n<sequence<T, First, Rest...>, N> {
    static_assert(N - 1 <= sizeof...(Rest), "sequence index out of bounds");
    using type = typename remove_first_n<sequence<T, Rest...>, N - 1>::type;
};

#if __cplusplus >= 201402L
template <typename SequenceT, std::size_t N>
using remove_first_n_t = typename remove_first_n<SequenceT, N>::type;
#endif

namespace quicksort {

namespace detail {

template <typename SequenceT, typename Condition>
struct filter;

template <typename T, T... Values, typename Condition>
struct filter<sequence<T, Values...>, Condition> {
    using type = typename sequence_cat<typename conditional<
        Condition()(Values), sequence<T, Values>, sequence<T>>::type...>::type;
};

#if __cplusplus >= 201402L
template <typename SequenceT, typename Condition>
using filter_t = typename filter<SequenceT, Condition>::type;
#endif

}  // namespace detail

template <typename SequenceT>
struct sort;

template <typename T>
struct sort<sequence<T>> {
    using type = sequence<T>;
};

template <typename T, T Value>
struct sort<sequence<T, Value>> {
    using type = sequence<T, Value>;
};

template <typename T, T... Values>
struct sort<sequence<T, Values...>> {
   private:
    static constexpr T pivot =
        get<sequence<T, Values...>, sizeof...(Values) / 2>::value;

    using rest = typename sequence_cat<
        typename first_n<sequence<T, Values...>, sizeof...(Values) / 2>::type,
        typename remove_first_n<sequence<T, Values...>,
                                sizeof...(Values) / 2 + 1>::type>::type;

    struct less_equal {
        constexpr bool operator()(const T &value) { return value <= pivot; }
    };

    struct greater {
        constexpr bool operator()(const T &value) { return value > pivot; }
    };

   public:
    using type = typename sequence_cat<
        typename sort<typename detail::filter<rest, less_equal>::type>::type,
        sequence<T, pivot>,
        typename sort<typename detail::filter<rest, greater>::type>::type>::
        type;
};

#if __cplusplus >= 201402L
template <typename SequenceT>
using sort_t = typename sort<SequenceT>::type;
#endif

}  // namespace quicksort

namespace mergesort {

namespace detail {

template <typename SequenceLHS, typename SequenceRHS>
struct merge_sorted;

template <typename T>
struct merge_sorted<sequence<T>, sequence<T>> {
    using type = sequence<T>;
};

template <typename T, T... ValuesLHS>
struct merge_sorted<sequence<T, ValuesLHS...>, sequence<T>> {
    using type = sequence<T, ValuesLHS...>;
};

template <typename T, T... ValuesRHS>
struct merge_sorted<sequence<T>, sequence<T, ValuesRHS...>> {
    using type = sequence<T, ValuesRHS...>;
};

template <typename T, T FirstLHS, T... RestLHS, T FirstRHS, T... RestRHS>
struct merge_sorted<sequence<T, FirstLHS, RestLHS...>,
                    sequence<T, FirstRHS, RestRHS...>> {
    using type = typename sequence_cat<
        sequence<T, FirstLHS <= FirstRHS ? FirstLHS : FirstRHS>,
        typename merge_sorted<
            typename conditional<FirstLHS <= FirstRHS, sequence<T, RestLHS...>,
                                 sequence<T, FirstLHS, RestLHS...>>::type,
            typename conditional<FirstLHS <= FirstRHS,
                                 sequence<T, FirstRHS, RestRHS...>,
                                 sequence<T, RestRHS...>>::type>::type>::type;
};

#if __cplusplus >= 201402L
template <typename SequenceLHS, typename SequenceRHS>
using merge_sorted_t = typename merge_sorted<SequenceLHS, SequenceRHS>::type;
#endif

}  // namespace detail

template <typename SequenceT>
struct sort;

template <typename T>
struct sort<sequence<T>> {
    using type = sequence<T>;
};

template <typename T, T Value>
struct sort<sequence<T, Value>> {
    using type = sequence<T, Value>;
};

template <typename T, T... Values>
struct sort<sequence<T, Values...>> {
    using type = typename detail::merge_sorted<
        typename sort<typename first_n<sequence<T, Values...>,
                                       sizeof...(Values) / 2>::type>::type,
        typename sort<typename remove_first_n<
            sequence<T, Values...>, sizeof...(Values) / 2>::type>::type>::type;
};

#if __cplusplus >= 201402L
template <typename SequenceT>
using sort_t = typename sort<SequenceT>::type;
#endif

}  // namespace mergesort

}  // namespace metasort

#endif  // METASORT_HPP