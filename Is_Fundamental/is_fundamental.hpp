#ifndef IS_FUNDAMENTAL_HPP
#define IS_FUNDAMENTAL_HPP

#include <type_traits>

// Primary template declaration
template <typename T> struct is_fundamental : std::false_type {};

// Specializations for standard integral types
template <> struct is_fundamental<bool> : std::true_type {};

template <> struct is_fundamental<char> : std::true_type {};

template <> struct is_fundamental<unsigned char> : std::true_type {};

template <> struct is_fundamental<unsigned short> : std::true_type {};

template <> struct is_fundamental<unsigned int> : std::true_type {};

template <> struct is_fundamental<unsigned long> : std::true_type {};

template <> struct is_fundamental<unsigned long long> : std::true_type {};

template <> struct is_fundamental<int> : std::true_type {};

// Specializations for standard floating-point types
template <> struct is_fundamental<float> : std::true_type {};

template <> struct is_fundamental<double> : std::true_type {};

template <> struct is_fundamental<long double> : std::true_type {};

// Specialization for void type
template <> struct is_fundamental<void> : std::true_type {};

// Specialization for nullptr_t
template <> struct is_fundamental<std::nullptr_t> : std::true_type {};
#endif
