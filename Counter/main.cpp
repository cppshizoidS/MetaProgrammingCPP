#include <iostream>

template <unsigned N> struct reader {
  friend auto counted_flag(reader<N>);
};

template <unsigned N> struct setter {
  friend auto counted_flag(reader<N>) {}

  static constexpr unsigned n = N;
};

template <auto Tag, unsigned NextVal = 0>
[[nodiscard]] consteval auto counter_impl() {
  constexpr bool counted_past_value =
      requires(reader<NextVal> r) { counted_flag(r); };

  if constexpr (counted_past_value) {
    return counter_impl<Tag, NextVal + 1>();
  } else {
    setter<NextVal> s;
    return s.n;
  }
}

template <auto Tag = [] {}, auto Val = counter_impl<Tag>()>
constexpr auto counter = Val;

int main() {
  static_assert(counter<> == 0);
  static_assert(counter<> == 1);
  static_assert(counter<> == 2);
  static_assert(counter<> == 3);
  static_assert(counter<> == 4);
  static_assert(counter<> == 5);
  static_assert(counter<> == 6);
  static_assert(counter<> == 7);
  static_assert(counter<> == 8);
  static_assert(counter<> == 9);
  static_assert(counter<> == 10);
}
