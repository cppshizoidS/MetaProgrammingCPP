#include "is_same.hpp"

template<typename T, typename U>
concept SameAs = is_same::is_same_v<T,U>;

