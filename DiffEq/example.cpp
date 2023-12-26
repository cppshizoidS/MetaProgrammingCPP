#include "diff_methods.hpp"

int main(int argc, char *argv[]) {
  constexpr double x0 = 0, y0 = 1, xn = 10;
  constexpr double dx[] = {1e-1, 1e-2, 1e-3};
  auto f = [](double x, double y) { return x * x + 2; };
  auto x = create<vector>(x0, xn, dx[0]);
  auto em = euler_method<double, vector>(f, dx[0], x, y0);
  auto km = koshi_method<double, vector>(f, dx[0], x, y0);
  auto me = mod_euler_method<double, vector>(f, dx[0], x, y0);
  auto rk = runge_kutt_method<double, vector>(f, dx[0], x, y0);
  cout<< "euler method:\t";
  print(em);
  cout << "koshi method:\t";
  print(km);
  cout << "modif euler method:\t";
  print(me);
  cout << "runge kutt method:\t";
  print(rk);
  return 0;
}
