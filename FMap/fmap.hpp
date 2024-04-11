#ifndef FMAP_HPP
#define FMAP_HPP

#include <iterator>
#include <utility>
#include <vector>

template <typename F>
concept Functor = requires(F f) {
  { f(std::declval<int>()) } -> std::same_as<int>;
};

template <typename T>
concept Container = requires(T t) {
  { t.begin() } -> std::input_or_output_iterator;
  { t.end() } -> std::input_or_output_iterator;
};

template <Functor F, Container C> auto fmap(F &&f, const C &container) {
  using ResultType = std::invoke_result_t<
      F,
      typename std::iterator_traits<decltype(container.begin())>::value_type>;
  std::vector<ResultType> result;
  result.reserve(std::size(container));

  for (const auto &element : container) {
    result.push_back(std::forward<F>(f)(element));
  }

  return result;
}

#endif
