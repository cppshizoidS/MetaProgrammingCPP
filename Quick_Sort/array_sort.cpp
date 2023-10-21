#include <array>

template<typename T, std::size_t N>
constexpr void QuickSort(std::array<T, N> &array, std::size_t low, std::size_t high)
{
    if (high <= low) return;
    auto i = low, j = high + 1;
    auto key = array[low];
    for (;;) {
        while (array[++i] < key) if (i == high) break;
        while (array[--j] > key) if (j == low) break;
        if (i >= j) break;
  
        auto tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;

    }

    auto tmp = array[low];
    array[low] = array[j];
    array[j] = tmp;

    QuickSort(array, low, j - 1);
    QuickSort(array, j + 1, high);
}

template<typename T, std::size_t N>
constexpr std::array<T, N> QuickSort(std::array<T, N> array)
{
    QuickSort(array, 0, N - 1);
    return array;
}

int main()
{   
    constexpr std::array<int, 6> array12{7, 9, 3, 6, 1, 19};
    constexpr auto array13 = QuickSort(array12);
    return 0;
} 
