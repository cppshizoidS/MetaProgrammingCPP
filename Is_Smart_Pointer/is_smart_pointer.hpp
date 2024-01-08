#ifndef IS_SMART_POINTER
#define IS_SMART_POINTER

#include <concepts>

template <typename T>
concept SmartPointer = requires(T t) {
  { t.operator->() } -> std::convertible_to<typename T::element_type *>;
  { *t } -> std::convertible_to<typename T::element_type &>;
};

template <typename T> struct is_smart_pointer : std::false_type {};

template <SmartPointer T> struct is_smart_pointer<T> : std::true_type {};
#endif
