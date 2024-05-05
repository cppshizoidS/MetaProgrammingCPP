#include <pm/functional/match.h>
#include <vector>
#include <variant>
#include <string>
#include <iostream>

namespace std
{

template <typename Target, typename... Ts>
Target const* dyn_cast(variant<Ts...> const* v)
{
  return get_if<Target>(v);
}

template <typename Target, typename... Ts>
Target* dyn_cast(variant<Ts...>* v)
{
  return get_if<Target>(v);
}
}

int main()
{
  struct MyType {};
  using var_t = std::variant<double, int, MyType>;

  std::vector<var_t>  vars = { var_t{3.14}, var_t{1337}, var_t{MyType{}} };
  for (auto const& var : vars)
  {
    auto x = pm::match(var)
        (   [](double x) { return "float: " + std::to_string(x); }
         ,   [](int n) { return "int: " + std::to_string(n); }
         ,   []{ return std::string("<unknown type>"); }
        );
    std::cout << x << std::endl;
  }
}