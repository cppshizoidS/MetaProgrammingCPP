#pragma once

#include <concepts>
#include <iostream>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

namespace Impl {
template <typename T>
concept HasToString = requires(const T &object) {
  { object.to_string() } -> std::convertible_to<std::string>;
};

template <typename T>
concept HasStdConversion = requires(T number) { std::to_string(number); };

template <typename T>
concept IsContainer = !Impl::HasToString<T> &&
                      requires(const T &container) { std::begin(container); };

template <typename T>
concept IsString = IsContainer<T> && std::constructible_from<std::string, T>;

} // namespace Impl

template <Impl::HasToString Object> std::string makeString(Object &&object) {
  return object.to_string();
}

template <Impl::HasStdConversion Numeric>
std::string makeString(Numeric &&value) {
  return std::to_string(std::forward<Numeric>(value));
}

template <Impl::IsString String> std::string makeString(String &&s) {
  return std::string(std::forward<String>(s));
}

template <Impl::IsContainer Container>
std::string makeString(Container &&iterable) {
  std::string result;
  for (auto &&i : std::forward<Container>(iterable)) {
    if (!result.empty())
      result += ';';
    result += makeString(std::forward<decltype(i)>(i));
  }

  return result;
}

template <typename... Pack>
  requires(sizeof...(Pack) > 1)
std::string makeString(Pack &&...pack) {
  return (... += makeString(std::forward<Pack>(pack)));
}
