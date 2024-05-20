#include <print>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

template <typename T, typename = void>
struct has_name : std::false_type {};

template <typename T>
struct has_name<T, std::void_t<decltype(std::declval<T>().name)>>
    : std::true_type {};

template <typename T>
constexpr bool has_name_v = has_name<T>::value;

template <typename T, typename = void>
struct has_id : std::false_type {};

template <typename T>
struct has_id<T, std::void_t<decltype(std::declval<T>().id)>> : std::true_type {
};

template <typename T>
constexpr bool has_id_v = has_id<T>::value;

template <typename T>
concept CustomType = has_name_v<T> && has_id_v<T>;

template <typename NameType, typename IdType, typename... MetadataTypes>
struct CustomTypeBuilder {
  NameType name;
  IdType id;
  std::tuple<MetadataTypes...> metadata;

  [[maybe_unused]] CustomTypeBuilder(NameType name, IdType id,
                                     MetadataTypes... metadata)
      : name(std::move(name)), id(id), metadata(std::make_tuple(metadata...)) {}

  constexpr void display() const {
    std::print("Name: {}, ID: {}\n", name, id);
    std::apply(
        [](const auto&... args) {
          ((std::print("Metadata: {}\n", args)), ...);
        },
        metadata);
  }
};

int main() {
  CustomTypeBuilder<std::string, int, double, std::string> customType(
      "ExampleType", 123, 4.56, "Some metadata");
  customType.display();
  static_assert(CustomType<decltype(customType)>,
                "customType does not satisfy CustomType concept");

  return 0;
}
