//
// Created by cppshizoid on 4/14/24.
//
#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <cstddef>
#include <vector>

namespace tensor {
template <typename T, size_t N>
class tensor {
 public:
  constexpr explicit tensor(size_t size = 3);

  constexpr tensor<T, N - 1>& operator[](size_t k);
  constexpr const tensor<T, N - 1>& operator[](size_t k) const;

  template <typename... Args>
    requires(sizeof...(Args) < N) && (sizeof...(Args) >= 1)
  decltype(auto) operator[](size_t i, Args&&... args) {
    return operator[](i).operator[](args...);
  }

  constexpr void resize(size_t size);

  [[nodiscard]] constexpr size_t size() const { return elements.size(); }

 private:
  std::vector<tensor<T, N - 1>> elements;
};

template <typename T>
class tensor<T, 1> {
 public:
  constexpr explicit tensor(size_t size = 3);

  constexpr T& operator[](size_t k);
  constexpr const T& operator[](size_t k) const;

  constexpr void resize(size_t size);

  [[nodiscard]] constexpr size_t size() const { return elements.size(); }

 private:
  std::vector<T> elements;
};

template <typename T, size_t N>
constexpr tensor<T, N>::tensor(size_t size) {
  resize(size);
}

template <typename T, size_t N>
constexpr void tensor<T, N>::resize(size_t size) {
  elements.resize(size);

  for (auto& element : elements) element.resize(size);
}

template <typename T, size_t N>
constexpr tensor<T, N - 1>& tensor<T, N>::operator[](size_t k) {
  return elements[k];
}

template <typename T, size_t N>
constexpr const tensor<T, N - 1>& tensor<T, N>::operator[](size_t k) const {
  return elements[k];
}

template <typename T>
constexpr tensor<T, 1>::tensor(size_t size) {
  resize(size);
}

template <typename T>
constexpr void tensor<T, 1>::resize(size_t size) {
  elements.resize(size);
}

template <typename T>
constexpr T& tensor<T, 1>::operator[](size_t k) {
  return elements[k];
}

template <typename T>
constexpr const T& tensor<T, 1>::operator[](size_t k) const {
  return elements[k];
}
}  // namespace tensor

#endif
