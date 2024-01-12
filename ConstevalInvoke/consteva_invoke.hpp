#ifndef TOOLS_CONSTEVAL_ENVOKE_HPP
#define TOOLS_CONSTEVAL_ENVOKE_HPP

#include <functional>

namespace cppshizoid::tools {
template <typename... Param>
consteval decltype(auto) constexval_invoke(Param &&...param) {
  return std::invoke(std::forward<Param>(param)...);
}
} // namespace cppshizoid

#endif
