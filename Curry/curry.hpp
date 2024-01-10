#ifndef CURRY_HPP
#define CURRY_HPP

#include <functional>

namespace cppshizoid::tools {
constexpr decltype(auto) curry(auto f, auto... ps) {
  if constexpr (requires { std::invoke(f, ps...); }) {
    return std::invoke(f, ps...);
  } else {
    return [f, ps...](auto... qs) -> decltype(auto) {
      return curry(f, ps..., qs...);
    };
  }
}
} // namespace cppshizoid::tools
#endif
