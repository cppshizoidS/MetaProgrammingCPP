#include <print>
#include <tuple>
#include <utility>
#include <array>
#include <string>
#include <cassert>

/**
 * @brief Converts an object to a tuple if possible
 *
 * Function attempts to decompose an object into a tuple using structured
 * bindings. If the object can be decomposed into two elements, it returns a
 * tuple containing these elements. Otherwise, it returns an empty tuple.
 *
 * @tparam T The type of the object to be converted to a tuple
 * @param obj The object to be converted to a tuple
 * @return A tuple containing the decomposed elements of the object
 */
template <class T>
[[nodiscard]] constexpr auto to_tuple(T&& obj) {
    if constexpr (requires {
        [&obj]() -> decltype(auto) {
            auto&& [p1, p2] = obj;
            return std::tuple{p1, p2};
        };
    }) {
        auto&& [p1, p2] = std::forward<T>(obj);
        return std::tuple{p1, p2};
    } else {
        return std::tuple<>();
    }
}

/**
 * @brief Converts an object to a pair if possible
 *
 * Function attempts to decompose an object into a pair using structured
 * bindings. If the object can be decomposed into two elements, it returns a
 * pair containing these elements. Otherwise, it returns an empty pair.
 *
 * @tparam T The type of the object to be converted to a pair
 * @param obj The object to be converted to a pair
 * @return A pair containing the decomposed elements of the object
 */
template <class T>
[[nodiscard]] constexpr auto to_pair(T&& obj) {
    if constexpr (requires {
        [&obj]() -> decltype(auto) {
            auto&& [p1, p2] = obj;
            return std::pair{p1, p2};
        };
    }) {
        auto&& [p1, p2] = std::forward<T>(obj);
        return std::pair{p1, p2};
    } else {
        return std::pair<int, int>{};
    }
}

/**
 * @brief Converts an object to an array if possible
 *
 * Function attempts to decompose an object into an array using structured
 * bindings. If the object can be decomposed into two elements of the same type, 
 * it returns an array containing these elements. Otherwise, it returns an empty array.
 *
 * @tparam T The type of the object to be converted to an array
 * @param obj The object to be converted to an array
 * @return An array containing the decomposed elements of the object
 */
template <class T>
[[nodiscard]] constexpr auto to_array(T&& obj) {
    if constexpr (requires {
        [&obj]() -> decltype(auto) {
            auto&& [p1, p2] = obj;
            return std::array{p1, p2};
        };
    }) {
        auto&& [p1, p2] = std::forward<T>(obj);
        if constexpr (std::is_same_v<decltype(p1), decltype(p2)>) {
            return std::array{p1, p2};
        } else {
            return std::array<int, 2>{};
        }
    } else {
        return std::array<int, 2>{};
    }
}

/**
 * @brief Structure with integer and double
 */
struct MyStruct {
    int a;
    double b;
};

/**
 * @brief Structure representing a point with x and y coordinates
 */
struct Point {
    double x;
    double y;
};

/**
 * @brief Structure representing a person with a name and age
 */
struct Person {
    std::string name;
    int age;
};

/**
 * @brief Structure representing a rectangle with width and height
 */
struct Rectangle {
    double width;
    double height;
};

int main() {
    constexpr MyStruct s{1, 2.0};
    auto t = to_tuple(s);
    static_assert(!std::is_same_v<decltype(t), std::tuple<>>);
    std::print("MyStruct tuple elements: ({}, {})\n", std::get<0>(t), std::get<1>(t));

    auto p = to_pair(s);
    static_assert(!std::is_same_v<decltype(p), std::pair<int, int>>);
    std::print("MyStruct pair elements: ({}, {})\n", p.first, p.second);

    constexpr Point pt{3.0, 4.0};
    auto t2 = to_tuple(pt);
    static_assert(!std::is_same_v<decltype(t2), std::tuple<>>);
    std::print("Point tuple elements: ({}, {})\n", std::get<0>(t2), std::get<1>(t2));

    auto p2 = to_pair(pt);
    static_assert(!std::is_same_v<decltype(p2), std::pair<int, int>>);
    std::print("Point pair elements: ({}, {})\n", p2.first, p2.second);

    auto a2 = to_array(pt);
    static_assert(!std::is_same_v<decltype(a2), std::array<int, 2>>);
    std::print("Point array elements: ({}, {})\n", a2[0], a2[1]);

    const Person person{"Alice", 30};
    auto t3 = to_tuple(person);
    static_assert(!std::is_same_v<decltype(t3), std::tuple<>>);
    std::print("Person tuple elements: ({}, {})\n", std::get<0>(t3), std::get<1>(t3));

    auto p3 = to_pair(person);
    static_assert(!std::is_same_v<decltype(p3), std::pair<int, int>>);
    std::print("Person pair elements: ({}, {})\n", p3.first, p3.second);

    constexpr Rectangle rect{5.0, 10.0};
    auto t4 = to_tuple(rect);
    static_assert(!std::is_same_v<decltype(t4), std::tuple<>>);
    std::print("Rectangle tuple elements: ({}, {})\n", std::get<0>(t4), std::get<1>(t4));

    auto p4 = to_pair(rect);
    static_assert(!std::is_same_v<decltype(p4), std::pair<int, int>>);
    std::print("Rectangle pair elements: ({}, {})\n", p4.first, p4.second);

    auto a4 = to_array(rect);
    static_assert(!std::is_same_v<decltype(a4), std::array<int, 2>>);
    std::print("Rectangle array elements: ({}, {})\n", a4[0], a4[1]);

    return 0;
}
