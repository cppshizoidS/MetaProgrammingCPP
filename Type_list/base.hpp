#ifndef __BASE__
#define __BASE__

namespace types {
// compare types
template <typename U, typename T> struct same_t {
  static constexpr auto value = false;
};

template <typename U> struct same_t<U, U> {
  static constexpr auto value = true;
};

// ternary operator
template <bool, typename U, typename T> struct if_t {
  using value = U;
};

template <typename U, typename T> struct if_t<false, U, T> {
  using value = T;
};

// represetation of constants as types
template <int i> struct int2type {
  static constexpr auto value = i;
};

} // namespace types

#endif /* ifndef __BASE__ */
