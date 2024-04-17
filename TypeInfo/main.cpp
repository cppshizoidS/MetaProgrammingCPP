#include "type_info.hpp"
#include <print>

int main() {
  static_assert(Typeinfo<short>::isNumber == true,
                "Short type should be a number");
  static_assert(isPointer<short *>::value == true,
                "Short* should be a pointer");

  std::print("isPointer<short*>::value  = {}\n", isPointer<short *>::value);
  std::print("isPointer<short>::value   = {}\n", isPointer<short>::value);
  std::print("isPointer<double>::value  = {}\n", isPointer<double>::value);
  std::print("isPointer<double*>::value = {}\n", isPointer<double *>::value);

  std::print("isPointer<short*>()()  = {}\n", isPointer<short *>()());
  std::print("isPointer<short>()()   = {}\n", isPointer<short>()());
  std::print("isPointer<double>()()  = {}\n", isPointer<double>()());
  std::print("isPointer<double*>()() = {}\n", isPointer<double *>()());

  showTypeInfo<bool>();
  showTypeInfo<char>();
  showTypeInfo<std::string>();
  showTypeInfo<int>();
  showTypeInfo<short>();
  showTypeInfo<float>();
  showTypeInfo<double>();
  showTypeInfo<const char *>();
  showTypeInfo<float *>();
  showTypeInfo<double *>();
  showTypeInfo<const double &>();

  disp(Typeinfo<removePointer<double>::type>::name);
  disp(Typeinfo<removePointer<double *>::type>::name);
  disp(Typeinfo<removePointer<const char *>::type>::name);

  return 0;
}
