#ifndef GADT_HPP
#define GADT_HPP

#include <type_traits>
#include <variant>

struct Expr {
  virtual ~Expr() = default;
};

template <int N> struct Lit : Expr {
  static constexpr int value = N;
};

template <typename L, typename R> struct Add : Expr {
  L left;
  R right;
};

template <typename L, typename R> struct Mul : Expr {
  L left;
  R right;
};

template <typename T>
concept IsExpr = std::is_base_of_v<Expr, T>;

template <typename T>
concept IsList = requires { T::value; };

struct EvalVisitor {
  template <IsList T> int operator()(const T &lit) const { return lit.value; }

  template <IsExpr L, IsExpr R> int operator()(const Add<L, R> &add) const {
    return std::visit(*this, add.left) + std::visit(*this, add.right);
  }

  template <IsExpr L, IsExpr R> int operator()(const Mul<L, R> &mul) const {
    return std::visit(*this, mul.left) * std::visit(*this, mul.right);
  }
};

auto eval(const Expr &expr) { return std::visit(EvalVisitor{}, expr); }
#endif
