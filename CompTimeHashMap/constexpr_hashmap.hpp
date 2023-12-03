#ifndef CONSTEXPR_HASH_MAP_CONSTEXPR_HASH_MAP_HPP
#define CONSTEXPR_HASH_MAP_CONSTEXPR_HASH_MAP_HPP

#include <array>
#include <cstddef>
#include <iterator>
#include <utility>

namespace hmap::ct {
template <std::size_t N, typename K, typename V> class hash_map {
public:
  using key_type = K;
  using value_type = V;
  using size_type = decltype(N);
  using data_type = std::array<std::pair<K, V>, N>;
  using const_iterator = typename data_type::const_iterator;

  template <typename... E>
  explicit constexpr hash_map(E &&...elements) noexcept
      : data{std::forward<E>(elements)...} {
    static_assert(N > 0, "N should be positive");
    static_assert(N == sizeof...(elements),
                  "Elements size doesn't match expected size of a hash-map");
  }

  [[nodiscard]] constexpr const_iterator find(const K &key) const noexcept {
    return search<0, N>(key);
  }

  [[nodiscard]] constexpr std::pair<bool, const V &>
  at(const K &key) const noexcept {
    const auto it = find(key);

    if (it != cend()) {
      return {true, it->second};
    }

    return {false, {}};
  }

  [[nodiscard]] constexpr const V &operator[](const K &key) const noexcept {
    return find(key)->second;
  }

  [[nodiscard]] constexpr bool contains(const K &key) const noexcept {
    return search<0, N>(key) != cend();
  }

  [[nodiscard]] constexpr size_type size() const noexcept {
    return data.size();
  }

  [[nodiscard]] constexpr const_iterator begin() const noexcept {
    return cbegin();
  }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
    return std::cbegin(data);
  }

  [[nodiscard]] constexpr const_iterator end() const noexcept { return cend(); }

  [[nodiscard]] constexpr const_iterator cend() const noexcept {
    return std::cend(data);
  }

  [[nodiscard]] constexpr bool empty() const noexcept { return false; }

protected:
  using index_type = size_type;

  template <index_type L, index_type R>
  [[nodiscard]] constexpr const_iterator search(const K &key) const noexcept {
    if constexpr (L < R) {
      if (equal(data[L].first, key)) {
        return std::next(cbegin(), L);
      }
      return search<L + 1, R>(key);
    }

    return cend();
  }

  template <typename T = K>
  [[nodiscard]] constexpr bool equal(const T &lhs,
                                     const T &rhs) const noexcept {
    return lhs == rhs;
  }

  [[nodiscard]] constexpr bool equal(const char *lhs,
                                     const char *rhs) const noexcept {
    return *lhs == *rhs && (*lhs == '\0' || equal(lhs + 1, rhs + 1));
  }

private:
  data_type data;
};
} // namespace hmap::ct

#endif // CONSTEXPR_HASH_MAP_CONSTEXPR_HASH_MAP_HPP
