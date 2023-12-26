#ifndef __DIFF_METHODS__
#define __DIFF_METHODS__

#include <functional>
#include <iostream>
#include <vector>

using std::cout, std::endl, std::vector, std::function;

template <class T> void print(T &&x) {
  for (auto &&i : x)
    cout << i << " ";
  cout << endl;
}

template <template <typename...> class T, typename D, typename... Ts>
T<D, Ts...> create(D a, D b, D dx) {
  T<D, Ts...> c;
  for (auto i = a; i < b; i += dx)
    c.push_back(i);
  return c;
}

template <typename D, template <typename...> class T, typename... Ts>
T<D, Ts...> diff(function<D(D, D)> next_y, T<D, Ts...> x, D y0) {
  auto yi = y0;
  T<D, Ts...> y;
  for (auto xi : x) {
    y.push_back(yi);
    yi = next_y(xi, yi);
  }
  return y;
}

template <typename D, template <typename...> class T, typename... Ts>
T<D, Ts...> euler_method(function<D(D, D)> f, D dx, T<D, Ts...> x, D y0) {
  return diff<D, T>([&f, dx](D x, D y) -> D { return y + dx * f(x, y); }, x,
                    y0);
}

template <typename D, template <typename...> class T, typename... Ts>
T<D, Ts...> mod_euler_method(function<D(D, D)> f, D dx, T<D, Ts...> x, D y0) {
  return diff<D, T>(
      [&f, dx](D x, D y) -> D {
        auto h = dx / 2;
        return y + dx * f(x + h, y + h * f(x, y));
      },
      x, y0);
}

template <typename D, template <typename...> class T, typename... Ts>
T<D, Ts...> koshi_method(function<D(D, D)> f, D dx, T<D, Ts...> x, D y0) {
  return diff<D, T>(
      [&f, dx](D x, D y) -> D {
        auto k1 = dx * f(x, y);
        return y + 0.5 * (k1 + dx * f(x + dx, y + k1));
      },
      x, y0);
}

template <typename D, template <typename...> class T, typename... Ts>
T<D, Ts...> runge_kutt_method(function<D(D, D)> f, D dx, T<D, Ts...> x, D y0) {
  return diff<D, T>(
      [&f, dx](D x, D y) -> D {
        auto h2 = dx / 2;
        auto k1 = f(x, y);
        auto k2 = f(x + h2, y + h2 * k1);
        auto k3 = f(x + h2, y + h2 * k2);
        auto k4 = f(x + dx, y + dx * k3);
        return y + (dx / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
      },
      x, y0);
}

#endif /* ifndef __DIFF_METHODS__ */
