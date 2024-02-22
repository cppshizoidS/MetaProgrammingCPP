#ifndef IS_SPECIALIZATION_OF_H
#define IS_SPECIALIZATION_OF_H

#include <type_traits>

// a metafunction to check specialization
template <typename T, template <typename...> class Template>
struct is_specialization_of : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

#endif // IS_SPECIALIZATION_OF_H
