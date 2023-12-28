#include <coroutine>
#include <type_traits>

template <typename Type> struct is_coroutine_handle : std::false_type {};
// is_coroutine_handle
template <typename Promise>
struct is_coroutine_handle<std::coroutine_handle<Promise>> : std::true_type {};
// is_valid_await_suspend_return_type
template <typename Type>
struct is_valid_await_suspend_return_type
    : std::disjunction<std::is_void<Type>, std::is_same<Type, bool>,
                       is_coroutine_handle<Type>> {};

// await_suspend
template <typename Type>
using is_await_suspend_method = is_valid_await_suspend_return_type<
    decltype(std::declval<Type>().await_suspend(
        std::declval<std::coroutine_handle<>>()))>;

// await_ready
template <typename Type>
using is_await_ready_method =
    std::is_constructible<bool, decltype(std::declval<Type>().await_ready())>;

template <typename Type, typename = std::void_t<>>
struct is_awaitable : std::false_type {};

template <typename Type>
struct is_awaitable<Type,
                    std::void_t<decltype(std::declval<Type>().await_ready()),
                                decltype(std::declval<Type>().await_suspend(
                                    std::declval<std::coroutine_handle<>>())),
                                decltype(std::declval<Type>().await_resume())>>
    : std::conjunction<is_await_ready_method<Type>,
                       is_await_suspend_method<Type>> {};

template <typename Type>
constexpr bool is_awaitable_v = is_awaitable<Type>::value;
