#pragma once

#include <bits/utility.h>
#include <cstddef>
#include <utility>

namespace lib {
template <auto N> constexpr auto get_element(auto... args) {
  return [&]<std::size_t... Indexes>(std::index_sequence<Indexes...>) {
    return [](decltype((void *)Indexes)... DontCare, auto *arg,
              auto *...DontCareEither) { return *arg; }(&args...);
  }(std::make_index_sequence<N>{});
}
} // namespace lib
