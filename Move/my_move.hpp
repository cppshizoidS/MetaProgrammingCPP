#include "remove_reference.hpp"

template <typename T> remove_reference_t<T> &&move(T &&arg) noexcept {
  return static_cast<remove_reference_t<T> &&>(arg);
}
