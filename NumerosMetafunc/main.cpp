#include "meta.hpp"
#include <iostream>
#include <tuple>
#include <vector>

int main() {
  // Example 1: Sum type
  using SumResult = meta::sum_type_t<int, double, float>;
  std::cout << "Sum type result: " << typeid(SumResult).name() << std::endl;

  // Example 2: Remove const and reference
  using OriginalType = const int &;
  using ModifiedType = meta::remove_cvref_t<OriginalType>;
  std::cout << "Original type: " << typeid(OriginalType).name() << std::endl;
  std::cout << "Modified type: " << typeid(ModifiedType).name() << std::endl;

  // Example 3: Check if int is in the pack
  std::cout << "Is int in the pack? "
            << meta::is_type_in_pack_v<int, double, char, int> << std::endl;

  // Example 4: Peel the first type from a pack
  using FirstType = meta::peel_first_t<int, double, char>;
  std::cout << "First type in the pack: " << typeid(FirstType).name()
            << std::endl;

  // Example 5: Check if the pack is uniform
  std::cout << "Is the pack uniform? "
            << meta::is_pack_uniform_v<int, int, int> << std::endl;

  // Example 6: Check if the pack has only one type
  std::cout << "Is the pack only one type? "
            << meta::is_pack_only_v<int, int, int> << std::endl;

  // Example 7: Check if a type is iterable
  std::cout << "Is vector<int> iterable? "
            << meta::is_iterable_v<std::vector<int>> << std::endl;

  // Example 8: Check if a type is a forward iterator
  std::cout << "Is vector<int>::iterator a forward iterator? "
            << meta::is_forward_v<std::vector<int>::iterator> << std::endl;

  // Example 9: Check if a type is a tuple
  std::cout << "Is std::tuple<int, double> a tuple? "
            << meta::is_tuple_v<std::tuple<int, double>> << std::endl;

  // Example 10: Check if a type is a smart pointer
  std::cout << "Is std::unique_ptr<int> a smart pointer? "
            << meta::is_smart_pointer_v<std::unique_ptr<int>> << std::endl;

  // Example 11: Check if a type is printable
  std::cout << "Is int printable? " << meta::is_printable_v<int> << std::endl;

  // Example 12: Check if types are equality comparable
  std::cout << "Are int and double equality comparable? "
            << meta::are_equality_comparable_v<int, double> << std::endl;

  // Example 13: Apply binary partial apply
  using PartialApplyResult =
      meta::binary_partial_apply<std::is_same, int>::func_t<double>;
  std::cout << "Partial apply result: " << PartialApplyResult::value
            << std::endl;

  return 0;
}
