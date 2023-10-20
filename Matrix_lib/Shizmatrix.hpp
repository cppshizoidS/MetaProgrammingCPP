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
