#include <print>

#include "tensor.hpp"

int main() {
  tensor::tensor<int, 2> myTensor(3);

  for (size_t i = 0; i < myTensor.size(); ++i)
    for (size_t j = 0; j < myTensor[i].size(); ++j)
      myTensor[i][j] = i * myTensor.size() + j;

  std::print("Tensor:\n");
  for (size_t i = 0; i < myTensor.size(); ++i) {
    for (size_t j = 0; j < myTensor[i].size(); ++j) {
      std::print("{} ", myTensor[i][j]);
    }
    std::println(" ");
  }

  myTensor.resize(4);

  std::print("\nResized Tensor:\n");
  for (size_t i = 0; i < myTensor.size(); ++i) {
    for (size_t j = 0; j < myTensor[i].size(); ++j) {
      std::print("{} ", myTensor[i][j]);
    }
    std::println(" ");
  }

  return 0;
}
