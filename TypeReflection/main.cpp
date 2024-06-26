#include <tuple>
#include <print>
#include <ranges>

template <auto I>
struct Wrapper {};

template <typename... Ts>
struct TypeList {};

template <typename T>
struct TypeList<T> {
  using Type = T;
};

template <typename... Ts, typename... TTs>
consteval auto operator==(const TypeList<Ts...>&, const TypeList<TTs...>&) -> bool {
  return false;
};
template <typename... Ts>
consteval auto operator==(const TypeList<Ts...>&, const TypeList<Ts...>&) -> bool {
  return true;
};

template <typename... Ts>
inline constexpr TypeList<Ts...> kTypeList;

namespace impl {

template <std::size_t I, typename T>
struct IndexedType {};

template <typename...>
struct Caster {};

template <std::size_t... Is, typename... Ts>
struct Caster<std::index_sequence<Is...>, Ts...> : IndexedType<Is, Ts>... {};
} // namespace impl


template <std::size_t I, typename... Ts>
consteval auto Get(TypeList<Ts...>) -> decltype(
  []<typename T>(impl::IndexedType<I, T>&&) -> T {
}(impl::Caster<std::index_sequence_for<Ts...>, Ts...>{}));


template <auto I>
struct Getter {
  friend constexpr auto Magic(Getter<I>);
};

template <auto I, auto Value>
struct Injector {
  friend constexpr auto Magic(Getter<I>) {return Value;};
};

static_assert((std::ignore = Injector<0, 42>(), true));

static_assert(Magic(Getter<0>{}) == 42);


template <typename Main, auto I>
struct Caster {
  template <typename T, auto = Injector<TypeList<Main, Wrapper<I>>{}, TypeList<T>{}>{}>
  constexpr operator T&&(); 

  template <typename T, auto = Injector<TypeList<Main, Wrapper<I>>{}, TypeList<T>{}>{}>
  constexpr operator T&(); 
};


struct SimpleCaster {
  template <typename T>
  constexpr operator T&&();

  template <typename T>
  constexpr operator T&();
};

template <typename T, std::size_t Max, std::size_t... Is>
consteval auto GetArgsCountImpl() {
  if constexpr(requires{T{(Is, SimpleCaster{})...};}) {
    return sizeof...(Is);
  } else {
    static_assert(sizeof...(Is) != Max, "Not found counstructor");
    return GetArgsCountImpl<T, Is..., 0>();
  };
};

template <typename T, std::size_t Max = 256>
consteval auto GetArgsCount() {
  return GetArgsCountImpl<T, Max, 0, 0>();
};


template <typename T, std::size_t I = GetArgsCount<T>()>
consteval auto Reflect() {
  return [&]<auto... Is>(std::index_sequence<Is...>) requires requires {T{Caster<T, Is>{}...};} {
    return TypeList<typename decltype(Magic(Getter<TypeList<T, Wrapper<Is>>{}>{}))::Type...> {};
  }(std::make_index_sequence<I>());
};

struct SomeInterface {
  virtual auto SomeFunction() -> int = 0;
};

struct SomeInterface2 {
  virtual auto SomeFunction2() -> void = 0;
};

class SomeStruct {
public:
  SomeStruct(SomeInterface& some, SomeInterface2& other) :
                   some(some),
                   other(other) {
    this->some.SomeFunction();
  };

private:
  SomeInterface& some;
  SomeInterface2& other;
};

static_assert(Reflect<SomeStruct>() == kTypeList<SomeInterface, SomeInterface2>);

template <typename Tag, auto Value>
struct TagWithValue {};

template <typename Tag, std::size_t I = 0, typename... Ts>
consteval auto CounterImpl() -> std::size_t {
  if constexpr(requires{Magic(Getter<TagWithValue<Tag, I>{}>{});}) {
    return CounterImpl<Tag, I + 1, Ts...>();
  };
  return (std::ignore = Injector<TagWithValue<Tag, I>{}, 0>{}, I);
};

template <typename Tag = void, typename... Ts, auto R = CounterImpl<Tag, 0, Ts...>()>
consteval auto Counter() -> std::size_t {
  return R;
};

static_assert(Counter<void>() == 0);
static_assert(Counter<void, int>() == 1);
static_assert(Counter<void, void>() == 2);

static_assert(Counter() == 0);
static_assert(!(Counter() == 1));
static_assert(!(Counter() != Counter()));

template <typename Tag, std::size_t Index>
struct GetUniqueKey {};

template <typename... Ts>
consteval auto GetUnique(TypeList<Ts...>) {
  ([]{
    constexpr auto I = Counter<TypeList<Ts...>, Ts>();
    std::ignore = Injector<GetUniqueKey<TypeList<Ts...>, I>{}, TypeList<Ts>{}>{};
  }(), ...);
  return []<std::size_t... Is>(std::index_sequence<Is...>) {
    return TypeList<typename decltype(Magic(Getter<GetUniqueKey<TypeList<Ts...>, Is>{}>{}))::Type...>{};
  }(std::make_index_sequence<Counter<TypeList<Ts...>>()>());
};

static_assert(GetUnique(kTypeList<int, int>) == kTypeList<int>);

template <typename T, auto... Args>
inline constexpr auto Use() {
  std::ignore = T{Args...};
};

template <typename...>
consteval auto Ignore() {};

template <typename Current, std::size_t I>
struct InfoKey {};


template <typename Current>
struct InfoInjector {
  template <
    typename T,
    typename... Args,
    std::size_t I = Counter<Current, TypeList<T, Args...>>(),    
    auto = Injector<InfoKey<Current, I>{}, TypeList<T, Args...>{}>{}  
  >
  static auto Method(Args...) -> void;
};

template <typename T>
consteval auto Inject() {
  Ignore<decltype(Use<T, InfoInjector<T>{}>)>();
};

template <typename T, typename... Ts>
consteval auto GetTFromMethod() {
  return []<std::size_t... Is>(std::index_sequence<Is...>){
     return TypeList<decltype(Get<0>(Magic(Getter<InfoKey<T, Is>{}>{})))...>{};
  }(std::make_index_sequence<Counter<T>()>());
};

template <typename T, typename... Ts>
consteval auto DropHead(TypeList<T, Ts...>) -> TypeList<Ts...> {
  return {};
};

template <typename T>
consteval auto GetArgsFromMethod() {
  return []<std::size_t... Is>(std::index_sequence<Is...>) {
    return TypeList<decltype(DropHead(Magic(Getter<InfoKey<T, Is>{}>{})))...>{};
  }(std::make_index_sequence<Counter<T>()>());
};

struct SomeImplA {
  template <typename T, typename... Args>
  auto Method(Args...) {};
};

struct Foo {
  constexpr Foo(auto& a) {
    a.template Method<int>(42);
    std::println("");
  };
};

static_assert((Inject<Foo>(), 1));

static_assert(std::same_as<decltype(GetTFromMethod<Foo>()), TypeList<int>>);

static_assert(std::same_as<decltype(GetArgsFromMethod<Foo>()), TypeList<TypeList<int>>>);

struct Types {};


template <std::size_t Id>
struct MetaInfoKey {};


template <typename T>
struct MetaInfo {
  static constexpr std::size_t kTypeId = Counter<Types, T>();
  using Type = T;
private: 
  static constexpr auto _ = Injector<MetaInfoKey<kTypeId>{}, TypeList<T>{}>{};
};

template <typename T>
inline constexpr std::size_t kTypeId = MetaInfo<T>::kTypeId;

template <std::size_t Id>
using GetMetaInfo = MetaInfo<typename decltype(Magic(Getter<MetaInfoKey<Id>{}>{}))::Type>;

template <std::size_t Id>
using GetType = GetMetaInfo<Id>::Type;

constexpr auto array = std::array{kTypeId<int>, kTypeId<void>} | std::views::reverse;

static_assert(std::is_same_v<GetType<array[0]>, void>);


auto main() -> int {};
