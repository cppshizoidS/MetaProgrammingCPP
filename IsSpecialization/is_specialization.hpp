#ifndef IS_SPECIALIZATION_OF_HPP
#define IS_SPECIALIZATION_OF_HPP

#include <type_traits>

// a metafunction to check specialization
template <typename T, template <typename...> class Template>
struct is_specialization_of : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

template <typename T, template <typename...> class Template>
constexpr bool is_specialization_of_v = is_specialization_of<T, Template>::value;

#endif // IS_SPECIALIZATION_OF_HPP
