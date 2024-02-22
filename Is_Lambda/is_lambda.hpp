#ifndef IS_LAMBDA_HPP
#define IS_LAMBDA_HPP

#include <string_view>

namespace is_lambda
{
    template <typename... Args>
    constexpr decltype(auto) args_name()
    {   //pretty_func clang and gcc specific
        constexpr std::size_t len = 50;
        constexpr std::string_view name {__PRETTY_FUNCTION__ + len, sizeof(__PRETTY_FUNCTION__) - len - 3};

        return name;
    }

    template <typename... Args>
    inline constexpr auto args_name_v = args_name<Args...>();

    template <typename T>
    constexpr decltype(auto) type_name()
    {
        return args_name_v<std::__remove_cvref_t<T>>;
    }

    template <typename T>
    inline constexpr auto type_name_v = type_name<T>();

    template <typename T>
    constexpr bool is_lambda()
    {
        constexpr auto name = type_name_v<T>;
        constexpr auto size = name.size();

        if constexpr (size < 10 || name[size - 1] != '>' || name[size - 2] != ')')
            return false;

        auto n = size - 3;

        for (auto k{1}; k; --n)
        {
             n = name.find_last_of("()", n);
             k += 2 * (name[n] != '(') - 1;
        }

        return n >= 6 && name.substr(n - 6, 7) == "<lambda";
    }

    template <typename T>
    inline constexpr auto is_lambda_v = is_lambda<T>();

    template <auto F>
    constexpr bool is_lambda()
    {
        return is_lambda_v<decltype(F)>;
    }

    template <typename T>
    concept invocable = is_lambda_v<T> || std::is_invocable_v<T>;
}

#endif