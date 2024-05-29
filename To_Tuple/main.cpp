#include <print>
#include <tuple>
#include <utility>

/**
 * @brief Converts an objects to a tuple if possible
 *
 * Function attemps to decomposite object into a tuple using structured
 * bindings. If the object can be decomposited into two elements, it returns  a
 * tuple containing this elemetns. Otherwise, it triggers a static assestion
 * failure.
 *
 * @tparam T The type of the object to be converted to a tuple
 * @param obj The object to be converted to a tuple
 * @return A tuple containing the decomposed elements of the object
 * */

template <class T>
[[nodiscard]] constexpr auto to_tuple(T&& obj) {
  if constexpr (requires {
                  [&obj]() -> decltype(auto) {
                    auto&& [p1, p2] = obj;
                    return std::tuple{p1, p2};
                  };
                }) {
    auto&& [p1, p2] = std::forward<T>(obj);
    return std::tuple{p1, p2};
  } else {
   return std::tuple<>();
  }
}
/**
 * @brief Structure with integer and double
 * */
struct MyStruct {
  int a;
  double b;
};

int main() {
  constexpr MyStruct s{1, 2.0};
  auto t = to_tuple(s);

  std::print("Tuple elements: ({}, {})\n", std::get<0>(t), std::get<1>(t));

  return 0;
}
