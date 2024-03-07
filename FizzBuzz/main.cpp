#include <boost/type_index.hpp> // for pretty printing of types
#include <iostream>
#include <tuple>

struct Fizz {};
struct Buzz {};
struct FizzBuzz {};
template <size_t _N> struct number {
  constexpr static size_t N = _N;
};

template <typename Any>
concept has_N = requires { requires Any::N - Any::N == 0; };
template <typename A>
concept fizz_c = has_N<A> && requires { requires A::N % 3 == 0; };
template <typename A>
concept buzz_c = has_N<A> && requires { requires A::N % 5 == 0; };
template <typename A>
concept fizzbuzz_c = fizz_c<A> && buzz_c<A>;

template <typename X> struct res;
template <fizzbuzz_c X> struct res<X> {
  using result = FizzBuzz;
};
template <fizz_c X> struct res<X> {
  using result = Fizz;
};
template <buzz_c X> struct res<X> {
  using result = Buzz;
};
template <has_N X> struct res<X> {
  using result = X;
};

template <size_t cnt, typename... Args> struct concatenator;

template <size_t cnt, typename... Args>
struct concatenator<cnt, std::tuple<Args...>> {
  using type = typename concatenator<
      cnt - 1, std::tuple<typename res<number<cnt>>::result, Args...>>::type;
};

template <typename... Args> struct concatenator<0, std::tuple<Args...>> {
  using type = std::tuple<Args...>;
};

template <size_t Amount>
using fizz_buzz_full_template = typename concatenator<
    Amount - 1, std::tuple<typename res<number<Amount>>::result>>::type;

int main() {
  std::cout
      << boost::typeindex::type_id<fizz_buzz_full_template<100>>().pretty_name()
      << std::endl;
}
