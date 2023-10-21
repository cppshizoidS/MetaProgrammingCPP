#include <algorithm>
#include <forward_list>
#include <ios>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

template <class ForwardIt>
void quicksort(ForwardIt first, ForwardIt last) {
  if (first == last)
    return;

  auto pivot = *std::next(first, std::distance(first, last) / 2);
  auto middle1 = std::partition(first, last,
                                [pivot](const auto &em) { return em < pivot; });
  auto middle2 = std::partition(
      middle1, last, [pivot](const auto &em) { return !(pivot < em); });

  quicksort(first, middle1);
  quicksort(middle2, last);
}

auto l = []<class Check, class A, class... Any>() -> bool {
  return (std::is_same_v<Check, A> && ((std::is_same_v<Check, Any>), ...));
};

int main() {
  std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::cout << "Original vector: ";
  for (int elem : v)
    std::cout << elem << ' ';

  auto it =
      std::partition(v.begin(), v.end(), [](int i) { return i % 2 == 0; });

  std::cout << "\nPartitioned vector: ";
  std::copy(std::begin(v), it, std::ostream_iterator<int>(std::cout, " "));
  std::cout << "* ";
  std::copy(it, std::end(v), std::ostream_iterator<int>(std::cout, " "));

  std::forward_list<int> fl{1, 30, -4, 3, 5, -4, 1, 6, -8, 2, -5, 64, 1, 92};
  std::cout << "\nUnsorted list: ";
  for (int n : fl)
    std::cout << n << ' ';

  quicksort(std::begin(fl), std::end(fl));
  std::cout << "\nSorted using quicksort: ";
  for (int fi : fl)
    std::cout << fi << ' ';
  std::cout << '\n';

  std::cout << std::boolalpha
            << l.template operator()<int, int32_t, decltype(l)>() << std::endl;
  return 0;
}
