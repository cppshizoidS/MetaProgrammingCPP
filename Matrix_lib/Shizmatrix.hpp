#ifndef SHIZMATRIX_SHIZMATRIX_HPP
#define SHIZMATRIX_SHIZMATRIX_HPP

#include <cassert>
#include <ostream>
#include <type_traits>
#include <vector>

namespace shizmatrix {

template <typename T>
struct eval_return_type {
  using type = typename T::EvalReturnType;
};

template <typename T>
using eval_return_type_t = typename eval_return_type<T>::type;

template <typename T>
struct element_type {
  using type = typename T::ElementType;
};

template <typename T>
using element_type_t = typename element_type<T>::type;

template <typename T>
struct storage_type {
  using type = T const &;
};

template <typename T>
using storage_type_t = typename storage_type<T>::type;

template <typename E>
class expression {
public:

  inline E const &get_const_derived() const {
    return static_cast<E const &>(*this);
  }

  inline std::size_t num_rows() const {
    return get_const_derived().num_rows();
  }

  inline std::size_t num_cols() const {
    return get_const_derived().num_cols();
  }
};

template <typename T>
using enable_if_not_expression = std::enable_if_t<!std::is_base_of<expression<T>, T>::value>;

template <typename T>
class scalar_expression : public expression<scalar_expression<T>> {
private:

  T scalar;

public:

  using EvalReturnType = T;

  using ElementType = T;

  inline scalar_expression(T scalar) : scalar(scalar) {}

  inline T operator()(std::size_t, std::size_t) const {
    return scalar;
  }

  inline T eval() const {
    return scalar;
  }

  inline std::size_t num_rows() const {
    return 0;
  }

  inline std::size_t num_cols() const {
    return 0;
  }
};

template <typename T>
class matrix : public expression<matrix<T>> {
private:

  std::vector<T> container;

  std::size_t n_rows;

  std::size_t n_cols;

public:

  using EvalReturnType = matrix<T>;

  using ElementType = T;

  inline matrix() = default;

  inline matrix(std::size_t n_rows, std::size_t n_cols)
      : container(n_rows * n_cols), n_rows(n_rows), n_cols(n_cols) {}

  inline matrix(std::size_t n_rows, std::size_t n_cols, T fill)
      : container(n_rows * n_cols, fill), n_rows(n_rows), n_cols(n_cols) {}

  inline matrix(matrix<T> &&other)
      : container(std::move(other.container)), n_rows(other.num_rows()), n_cols(other.num_cols()) {}

  template <typename E>
  inline matrix(expression<E> const &other)
      : container(other.num_rows() * other.num_cols()), n_rows(other.num_rows()),
        n_cols(other.num_cols()) {
    assign(other.get_const_derived());
  }

  template <typename E>
  inline matrix &operator=(expression<E> const &other) {
    assert(n_rows >= other.num_rows());
    assert(n_cols >= other.num_cols());
    assign(other.get_const_derived());
    return *this;
  }

  inline matrix &operator=(matrix<T> &&other) {
    if (this != &other) {
      container = std::move(other.container);
      n_rows = other.n_rows;
      n_cols = other.n_cols;
    }
    return *this;
  }

  inline T operator()(std::size_t i, std::size_t j) const {
    assert(i < n_rows && i >= 0);
    assert(j < n_cols && j >= 0);
    return container[i * n_cols + j];
  }

  inline std::vector<T> &get_container() {
    return container;
  }

  inline std::size_t num_rows() const {
    return n_rows;
  }

  inline std::size_t num_cols() const {
    return n_cols;
  }

  inline const matrix<T> &eval() const {
    return *this;
  }

  inline void set_elt(std::size_t i, std::size_t j, T value) {
    container[i * n_cols + j] = value;
  }

  template <typename E>
  inline void assign(expression<E> const &expr) {
    for (std::size_t i = 0; i < n_rows; ++i) {
      for (std::size_t j = 0; j < n_cols; ++j) {
        container[i * n_cols + j] = expr.get_const_derived()(i, j);
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &ostream, const matrix<T> &mat) {
    for (std::size_t i = 0; i < mat.n_rows; ++i) {
      for (std::size_t j = 0; j < mat.n_cols; ++j) {
        ostream << mat(i, j) << ", ";
      }
      ostream << "\n";
    }
    return ostream;
  }

  template <typename E>
  inline matrix<T> &operator+=(expression<E> const &expr);

  template <typename E>
  inline matrix<T> &operator*=(expression<E> const &expr);

  template <typename E>
  inline matrix<T> &operator-=(expression<E> const &expr);

  template <typename Scalar, typename = enable_if_not_expression<Scalar>>
  inline matrix<T> &operator+=(Scalar const &expr);

  template <typename Scalar, typename = enable_if_not_expression<Scalar>>
  inline matrix<T> &operator-=(Scalar const &expr);

  template <typename Scalar, typename = enable_if_not_expression<Scalar>>
  inline matrix<T> &operator*=(Scalar const &expr);
};
} 

namespace std {
using namespace shizmatrix;

template <typename T1, typename T2>
struct common_type<matrix<T1>, matrix<T2>> {
  using type = matrix<std::common_type_t<T1, T2>>;
};

template <typename T1, typename T2>
struct common_type<matrix<T1>, T2> {
  using type = matrix<std::common_type_t<T1, T2>>;
};
} 
