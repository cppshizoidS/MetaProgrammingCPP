#include <array>
#include <cstdint>
#include <print>
#include <type_traits>

template <typename Head, typename... Tail>
struct MaxSizeHelper {
  static constexpr std::size_t MaxSize() {
    return std::max(sizeof(Head), MaxSizeHelper<Tail...>::MaxSize());
  }
};

template <typename Head>
struct MaxSizeHelper<Head> {
  static constexpr std::size_t MaxSize() { return sizeof(Head); }
};

template <typename... Types>
constexpr std::size_t MaxSize() {
  return MaxSizeHelper<Types...>::MaxSize();
}

template <typename Head, typename... Tail>
struct MaxAlignmentHelper {
  static constexpr std::size_t MaxAlignment() {
    return std::max(alignof(Head), MaxAlignmentHelper<Tail...>::MaxAlignment());
  }
};

template <typename Head>
struct MaxAlignmentHelper<Head> {
  static constexpr std::size_t MaxAlignment() { return alignof(Head); }
};

template <typename... Types>
constexpr std::size_t MaxAlignment() {
  return MaxAlignmentHelper<Types...>::MaxAlignment();
}

template <typename Needle, typename HayStackHead, typename... HayStackTail>
struct FindHelper {
  static constexpr std::size_t Find() {
    return 1 + FindHelper<Needle, HayStackTail...>::Find();
  }
};

template <typename Needle, typename... HayStackTail>
struct FindHelper<Needle, Needle, HayStackTail...> {
  static constexpr std::size_t Find() { return 0; }
};

template <typename Needle, typename... HayStack>
constexpr std::size_t Find() {
  return FindHelper<Needle, HayStack...>::Find();
}

template <std::size_t I, typename... Types>
struct ByIndex {};

template <std::size_t I, typename TypesHead, typename... TypesTail>
struct ByIndex<I, TypesHead, TypesTail...> {
  using Type = typename ByIndex<I - 1, TypesTail...>::Type;
};

template <typename TypesHead, typename... TypesTail>
struct ByIndex<0, TypesHead, TypesTail...> {
  using Type = TypesHead;
};

template <typename T>
constexpr void Destroy(void *ptr) {
  std::println("{}", __PRETTY_FUNCTION__);
  // std::cout << __PRETTY_FUNCTION__ << std::endl;
  reinterpret_cast<T *>(ptr)->~T();
}

template <typename... T>
struct helper {};

template <typename... Types, std::size_t... I>
struct helper<std::tuple<Types...>, std::index_sequence<I...>> {
  static constexpr auto make_arr() {
    return std::array<void (*)(void *), sizeof...(I)>{
        &Destroy<typename ByIndex<I, Types...>::Type>...};
  }
};

template <typename... Types>
constexpr void Destroy(std::size_t typeIndex, void *ptr) {
  static constexpr std::array<void (*)(void *), sizeof...(Types)> arr =
      helper<std::tuple<Types...>,
             std::make_index_sequence<sizeof...(Types)>>::make_arr();
  arr[typeIndex](ptr);
}

template <typename... Types>
class Variant {
 public:
  template <typename Type>
  constexpr explicit Variant(Type &&value)
      : m_typeIndex(
            Find<std::remove_cv_t<std::remove_reference_t<Type>>, Types...>()) {
    new (m_storage) Type{std::forward<Type>(value)};
  }

  constexpr ~Variant() noexcept { Destroy<Types...>(m_typeIndex, m_storage); }

  template <typename Type>
  constexpr Variant &operator=(Type &&value) {
    this->~Variant();
    new (this) Variant{std::forward<Type>(value)};
  }

  template <typename Type, typename... TArgs>
  constexpr void Emplace(TArgs &&...args) {
    this->~Variant();
    m_typeIndex = Find<Type, Types...>();
    new (m_storage) Type{std::forward<TArgs>(args)...};
  }

 private:
  alignas(MaxAlignment<Types...>()) uint8_t m_storage[MaxSize<Types...>()];
  uint8_t m_typeIndex;
};

class Log {
 public:
  Log() { std::print("Log()\n"); }
  Log(const Log &other) { std::print("Log(const Log& other)\n"); }
  Log(Log &&other) noexcept { std::print("Log(Log&& other)\n"); }
  Log &operator=(const Log &other) {
    std::print("Log& operator = (const Log& other)\n");
    return *this;
  }
  Log &operator=(Log &&other) noexcept {
    std::print("Log& operator = (Log&& other)\n");
    return *this;
  }
  ~Log() noexcept { std::print("~Log()\n"); }

 private:
  alignas(64) uint8_t buffer[155];
};

int main() {
  Variant<int, Log> v{1};
  v.Emplace<Log>();
}
