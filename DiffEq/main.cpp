#include "diff_methods.hpp"

int main(int argc, char *argv[]) {
  constexpr double x0 = 0, y0 = 1, xn = 10;
  constexpr double dx[] = {1e-1, 1e-2, 1e-3};
  auto f = [](double x, double y) { return x * x + 2; };
  auto x = create<std::vector>(x0, xn, dx[0]);
  auto em = euler_method<double, std::vector>(f, dx[0], x, y0);
  auto km = koshi_method<double, std::vector>(f, dx[0], x, y0);
  auto me = mod_euler_method<double, std::vector>(f, dx[0], x, y0);
  auto rk = runge_kutt_method<double, std::vector>(f, dx[0], x, y0);
  std::print("{}", "euler method:\t");
  print(em);
  std::print("{}", "koshi method:\t");
  print(km);
  std::print("{}", "modif euler method:\t");
  print(me);
  std::print("{}", "runge kutt method:\t");
  print(rk);
  return 0;
}
