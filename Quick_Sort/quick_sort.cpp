// quicksort
#include <iostream>

namespace li {
template <int n> struct int32 {
  using type = int32;
  static constexpr int value = n;
};

template <int... In> struct list {
  using type = list;
  static constexpr size_t size = sizeof...(In);
};
using null = list<>;

template <int, typename> struct cons;
template <int I, int... In> struct cons<I, list<In...>> : list<I, In...> {};

template <typename> struct first;
template <int I, int... In> struct first<list<I, In...>> : int32<I> {};

template <typename, int> struct push;
template <int I, int... In> struct push<list<In...>, I> : list<In..., I> {};
template <typename, typename> struct append;
template <typename L1> struct append<L1, null> : L1 {};
template <typename L1, int I, int... In>
struct append<L1, list<I, In...>>
    : append<typename push<L1, I>::type, typename list<In...>::type> {};

#define FILTER_GEN(NAME, _OP_)                                                 \
  template <typename, int, bool> struct _##NAME##_h;                           \
  template <int target, int I>                                                 \
  struct _##NAME##_h<list<I>, target, true> : list<I> {};                      \
  template <int target, int I>                                                 \
  struct _##NAME##_h<list<I>, target, false> : null {};                        \
  template <int target, int I, int... In>                                      \
  struct _##NAME##_h<list<I, In...>, target, true>                             \
      : cons<I, typename _##NAME##_h<typename list<In...>::type, target,       \
                                     (first<typename list<In...>::type>::value \
                                          _OP_ target)>::type> {};             \
  template <int target, int I, int... In>                                      \
  struct _##NAME##_h<list<I, In...>, target, false>                            \
      : _##NAME##_h<typename list<In...>::type, target,                        \
                    (first<typename list<In...>::type>::value _OP_ target)> {  \
  };                                                                           \
                                                                               \
  template <typename L, int target>                                            \
  struct NAME : _##NAME##_h<L, target, (first<L>::value _OP_ target)> {};      \
  template <int target> struct NAME<null, target> : null {};

FILTER_GEN(less_eq, <=)
FILTER_GEN(greater, >)
#undef FILTER_GEN

template <typename> struct qsort;
template <> struct qsort<null> : null {};
template <int I, int... In>
struct qsort<list<I, In...>>
    : append<typename qsort<
                 typename less_eq<typename list<In...>::type, I>::type>::type,
             typename cons<
                 I, typename qsort<typename greater<typename list<In...>::type,
                                                    I>::type>::type>::type> {};

} // namespace li

template <int I, int... In> static void print_list(li::list<I, In...>) {
  std::cout << '(' << I;
  int _[] = {0, ((void)(std::cout << ", " << In), 0)...};
  std::cout << ")\n";
}
static void print_list(li::null) { std::cout << "()\n"; }
template <int... In> static void test() {
  using original = li::list<In...>;
  using sorted = li::qsort<original>;
  std::cout << "before: ";
  print_list(original());
  std::cout << "after : ";
  print_list(sorted());
  std::cout << '\n';
}

int main(void) {
  std::cout << "Compile Time quicksort \n";
  test<>();
  test<1>();
  test<8, 1>();
  test<1, 2, 5, 8, -3, 2, 100, 4, 9, 3, -8, 33, 21, 3, -4, -4, -4, -7, 2, 5, 1,
       8, 2, 88, 42, 956, 21, 27, 39, 55, 1, 4, -5, -31, 9>();
}
