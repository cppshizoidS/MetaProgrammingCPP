#include <pm/functional/overload.h>
#include <vector>
#include <variant>
#include <string>
#include <iostream>

int main()
{
  struct MyType {};
  using var_t = std::variant<double, int, MyType>;

  std::vector<var_t>  vars = { var_t{3.14}, var_t{1337}, var_t{MyType{}} };

  for (auto const& var : vars)
  {
    auto visitor = pm::overload
        (   [](double x) { return "float: " + std::to_string(x); }
         ,   [](int n)    { return "int: " + std::to_string(n); }
         ,   [](auto)     { return std::string("<unknown type>"); }
        );
    std::cout << std::visit(visitor, var) << std::endl;
  }
}