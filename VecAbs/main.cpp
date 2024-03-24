#include "vec_abs.hpp"
#include <iostream>

int main() {
  // Создание вектора типа double размера 3
  vec_abs<double, 3> vec1;

  // Создание вектора из массива
  std::array<double, 3> arr = {1.0, 2.0, 3.0};
  vec_abs<double, 3> vec2(arr);

  // Создание вектора из нескольких аргументов
  vec_abs<double, 3> vec3(4.0, 5.0, 6.0);

  // Проверка преобразуемости типов
  bool convertible =
      all_convertible_to<int, double,
                         float>(); // Возвращает true, если все типы могут быть
                                   // преобразованы в double

  // Вывод результатов
  std::cout << "Vector 1: ";
  for (const auto &elem : vec1.data) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  std::cout << "Vector 2: ";
  for (const auto &elem : vec2.data) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  std::cout << "Vector 3: ";
  for (const auto &elem : vec3.data) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  std::cout << "Are all types convertible to double? " << std::boolalpha
            << convertible << std::endl;

  return 0;
}
