#ifndef TYPE_HIERARCHY_H
#define TYPE_HIERARCHY_H

#include <boost/assert.hpp>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "../pm/meta/fold_add.h"
#include "../pm/meta/scan_add.h"

namespace pm {

namespace type_hierarchy_detail {
/**
 * @brief Метка уровня иерархии типов.
 *
 * Эта метка представляет собой уровень иерархии типов.
 */
template <int Level>
struct level_tag {};
/**
 * @brief Вычисление уровня иерархии для типа.
 *
 * Эта структура вычисляет уровень иерархии для заданного типа.
 */
template <typename T, typename LevelTag = level_tag<0>>
struct level_of;
/**
 * @brief Шаблон для определения уровня иерархии типов.
 *
 * Этот шаблон определяет уровень иерархии для заданного типа и уровня.
 */
template <typename T, int N>
struct level_of<T, level_tag<N>>
    : std::conditional_t<std::is_convertible<T, level_tag<N>>::value,
                         level_of<T, level_tag<N + 1>>,
                         std::integral_constant<int, N - 1>> {};
/**
 * @brief Специализация шаблона для типов, не связанных с уровнями.
 *
 * Эта специализация определяет уровень иерархии для типов, не связанных с уровнями.
 */
template <typename T>
struct level_of<T, level_tag<0>>
    : std::conditional_t<std::is_convertible<T, level_tag<0>>::value,
                         level_of<T, level_tag<1>>,
                         std::integral_constant<int, -1>> {};

template <>
struct level_of<void, level_tag<0>> : std::integral_constant<int, 0> {};

template <typename T>
constexpr int level_of_v = level_of<T>::value;
/**
 * @brief Шаблонная структура для хранения идентификатора иерархии типов.
 *
 * Эта структура содержит идентификатор иерархии типов для заданного конфига.
 */
template <typename Config, typename... T>
struct id_holder;

template <typename Config>
struct config_holder {};
/**
 * @brief Шаблонная структура для получения конфигурации типа.
 *
 * Эта структура используется для получения конфигурации типа.
 */
template <typename T>
struct get_config {
  template <typename Config>
  static constexpr auto deduce(config_holder<Config>*) -> Config;
  template <typename Config>
  static constexpr auto deduce(const config_holder<Config>*) -> Config;
  static constexpr auto deduce(void*) -> void;

  using type = decltype(deduce(std::declval<T*>()));
};
/**
 * @brief Шаблонный псевдоним типа для получения конфигурации типа.
 *
 * Этот псевдоним типа используется для получения конфигурации типа.
 */
template <typename T>
using get_config_t = typename get_config<T>::type;
/**
 * @brief Шаблонная структура для хранения идентификатора иерархии типов.
 *
 * Эта структура хранит идентификатор иерархии типов для заданного конфига и типа.
 */
template <typename Config>
struct id_holder<Config>
    : Config::base_type, level_tag<0>, config_holder<Config> {
 private:
  typename Config::id_t m_type_hierarchy_id__;
  template <typename, typename...>
  friend struct id_holder;

 protected:
  id_holder() = default;

 public:
  typename Config::id_t type_hierarchy_id__() const {
    return m_type_hierarchy_id__;
  }
};
/**
 * @brief Шаблонная структура для хранения идентификатора иерархии типов.
 *
 * Эта структура хранит идентификатор иерархии типов для производного типа от супертипа.
 *
 * @tparam Config Конфигурация типа.
 * @tparam Derived Производный тип.
 */
template <typename Config, typename Derived>
struct id_holder<Config, Derived, void> : id_holder<Config>, level_tag<1> {
 private:
  static const typename Config::id_t s_type_hierarchy__; /**< Идентификатор иерархии типов. */
  /**
   * @brief Инициализирует идентификатор иерархии типов.
   *
   * @return Идентификатор иерархии типов.
   */
  static typename Config::id_t type_hierarchy_init_id__();
  template <typename, typename...>
  friend struct id_holder;
  template <typename, typename, typename>
  friend struct convertible_to_impl;

 public:
  /**
   * @brief Конструктор для инициализации идентификатора иерархии типов.
   */
  id_holder() { id_holder::m_type_hierarchy_id__ = s_type_hierarchy__; }
};
/**
 * @brief Шаблонная структура для хранения идентификатора иерархии типов.
 *
 * Эта структура хранит идентификатор иерархии типов для производного типа от супертипа.
 *
 * @tparam Config Конфигурация типа.
 * @tparam Derived Производный тип.
 * @tparam SuperType Супертип.
 */
template <typename Config, typename Derived, typename SuperType>
struct id_holder<Config, Derived, SuperType>
    : SuperType, level_tag<level_of_v<SuperType> + 1> {
 private:
  static_assert(level_of_v<SuperType> <
                    typename Config::bits_per_level{}.size(),
                "Supported number of hierarchy levels exceeded.");
  static const typename Config::id_t s_type_hierarchy__;
  /**
   * @brief Инициализирует идентификатор иерархии типов.
   *
   * @return Идентификатор иерархии типов.
   */
  static typename Config::id_t type_hierarchy_init_id__();
  template <typename, typename...>
  friend struct id_holder;
  template <typename, typename, typename>
  friend struct convertible_to_impl;

 public:
  using super_t = id_holder<Config, Derived, SuperType>;
  /**
   * @brief Конструктор для инициализации идентификатора иерархии типов.
   *
   * @param args Аргументы для конструктора супертипа.
   */
  template <typename... Args>
  constexpr id_holder(Args&&... args) : SuperType{std::forward<Args>(args)...} {
    SuperType::m_type_hierarchy_id__ = s_type_hierarchy__;
  }
};
/**
 * @brief Шаблонная структура для создания массива с заданным размером во время компиляции.
 *
 * @tparam T Тип элементов массива.
 * @tparam N Размер массива.
 */
template <typename T, size_t N>
struct constexpr_array {
  T data[N];
};
/**
 * @brief Получает элемент из массива с помощью индекса во время компиляции.
 *
 * @tparam N Индекс элемента.
 * @tparam Ns Индексы элементов в массиве.
 * @param seq Последовательность целых чисел.
 * @return Элемент массива с указанным индексом.
 */
template <int N, int... Ns>
constexpr int get(std::integer_sequence<int, Ns...>) {
  return constexpr_array<int, sizeof...(Ns)>{{Ns...}}.data[N];
}
/**
 * @brief Выполняет поиск значения по индексу в последовательности сдвигов уровней.
 *
 * @tparam Config Конфигурация типа.
 * @tparam N Индекс значения в последовательности сдвигов уровней.
 * @return Значение по указанному индексу в последовательности сдвигов уровней.
 */
template <typename Config, int N>
constexpr int lookup_v = get<N>(typename Config::level_shifts{});
/**
 * @brief Вычисляет сдвиг для указанного типа в иерархии типов.
 *
 * @tparam Config Конфигурация типа.
 * @tparam T Тип, для которого нужно вычислить сдвиг.
 * @return Сдвиг для указанного типа в иерархии типов.
 */
template <typename Config, typename T>
constexpr typename Config::id_t shift_v = lookup_v<Config, level_of_v<T>>;
/**
 * @brief Вычисляет маску для указанного типа в иерархии типов.
 *
 * @tparam Config Конфигурация типа.
 * @tparam T Тип, для которого нужно вычислить маску.
 * @return Маска для указанного типа в иерархии типов.
 */
template <typename Config, typename T>
constexpr typename Config::id_t mask_v = (1 << shift_v<Config, T>)-1;
/**
 * @brief Генерирует следующий идентификатор для уровня иерархии типов.
 *
 * @tparam Config Конфигурация типа.
 * @tparam Level Уровень иерархии типов.
 * @return Следующий идентификатор для уровня иерархии типов.
 */
template <typename Config, int Level = 0>
typename Config::id_t next_id() {
  static typename Config::id_t id{};
  ++id;
  BOOST_ASSERT_MSG(id < (static_cast<typename Config::id_t>(1)
                         << get<Level>(typename Config::bits_per_level{})),
                   "Ids for level are exhausted.");
  return id;
}
/**
 * @brief Шаблонная структура для проверки приводимости типа к другому типу в иерархии типов.
 *
 * @tparam TargetType Целевой тип.
 * @tparam TargetConfig Конфигурация целевого типа.
 * @tparam SourceConfig Конфигурация исходного типа.
 */
template <typename TargetType, typename TargetConfig, typename SourceConfig>
struct convertible_to_impl {
  static constexpr bool apply(id_holder<SourceConfig> const&) { return false; }
};
/**
 * @brief Шаблонная структура для проверки приводимости типа к другому типу в иерархии типов.
 *
 * Этот шаблон специализируется для случая, когда исходный тип и целевой тип имеют одинаковую конфигурацию.
 *
 * @tparam TargetType Целевой тип.
 * @tparam Config Конфигурация типа.
 */
template <typename TargetType, typename Config>
struct convertible_to_impl<TargetType, Config, Config> {
  /**
   * @brief Проверяет, можно ли привести исходный тип к целевому типу.
   *
   * @param src Экземпляр исходного типа.
   * @return true Если исходный тип можно привести к целевому типу.
   * @return false Если исходный тип нельзя привести к целевому типу.
   */
  static constexpr bool apply(id_holder<Config> const& src) {
    return (mask_v<Config, TargetType> & src.type_hierarchy_id__()) ==
           TargetType::s_type_hierarchy__;
  }
};
/**
 * @brief Шаблонная структура для проверки приводимости типа к другому типу в иерархии типов.
 *
 * Этот шаблон специализируется для случая, когда исходный тип является базовым типом в иерархии.
 *
 * @tparam Config Конфигурация типа.
 */
template <typename Config>
struct convertible_to_impl<typename Config::base_type, void, Config> {
  static constexpr bool apply(id_holder<Config> const&) { return true; }
};
/**
 * @brief Проверяет, можно ли привести тип иерархии к целевому типу.
 *
 * @tparam TargetType Целевой тип.
 * @tparam SourceType Исходный тип.
 * @param x Экземпляр исходного типа.
 * @return true Если тип иерархии можно привести к целевому типу.
 * @return false Если тип иерархии нельзя привести к целевому типу.
 */
template <typename TargetType, typename SourceType,
          typename = std::enable_if_t<
              std::is_base_of<level_tag<0>, SourceType>::value>>
bool constexpr convertible_to(SourceType const& x) {
  using config_t = get_config_t<SourceType>;
  auto const& src = static_cast<id_holder<config_t> const&>(x);
  return convertible_to_impl<TargetType, get_config_t<TargetType>,
                             config_t>::apply(src);
}
/**
 * @brief Вычисляет инициализацию идентификатора иерархии типов для случая, когда
 *        производный тип не имеет супертипа.
 *
 * @tparam Config Конфигурация типа.
 * @tparam Derived Производный тип.
 * @return Идентификатор иерархии типов.
 */
template <typename Config, typename Derived>
typename Config::id_t
id_holder<Config, Derived, void>::type_hierarchy_init_id__() {
  static const typename Config::id_t id = next_id<Config, 0>();
  return id;
}
/**
 * @brief Статическая переменная, содержащая идентификатор иерархии типов для случая,
 *        когда производный тип не имеет супертипа.
 *
 * @tparam Config Конфигурация типа.
 * @tparam Derived Производный тип.
 */
template <typename Config, typename Derived>
const typename Config::id_t
    id_holder<Config, Derived, void>::s_type_hierarchy__ =
        type_hierarchy_init_id__();
/**
 * @brief Вычисляет инициализацию идентификатора иерархии типов для случая,
 *        когда производный тип имеет супертип.
 *
 * @tparam Config Конфигурация типа.
 * @tparam Derived Производный тип.
 * @tparam SuperType Супертип.
 * @return Идентификатор иерархии типов.
 */
template <typename Config, typename Derived, typename SuperType>
typename Config::id_t
id_holder<Config, Derived, SuperType>::type_hierarchy_init_id__() {
  constexpr auto shift = shift_v<Config, SuperType>;
  static const typename Config::id_t id =
      SuperType::type_hierarchy_init_id__() |
      (next_id<Config, level_of_v<SuperType>>() << shift);
  return id;
}
/**
 * @brief Статическая переменная, содержащая идентификатор иерархии типов для случая,
 *        когда производный тип имеет супертип.
 *
 * @tparam Config Конфигурация типа.
 * @tparam Derived Производный тип.
 * @tparam SuperType Супертип.
 */
template <typename Config, typename Derived, typename SuperType>
const typename Config::id_t
    id_holder<Config, Derived, SuperType>::s_type_hierarchy__ =
        type_hierarchy_init_id__();
/**
 * @brief Приводит указатель на объект иерархии типов к указателю на целевой тип, если это возможно.
 *
 * @tparam TargetType Целевой тип.
 * @tparam Config Конфигурация типа.
 * @param p Указатель на объект иерархии типов.
 * @return Указатель на объект целевого типа, если приведение возможно, иначе nullptr.
 */
template <typename TargetType, typename Config>
constexpr TargetType* dyn_cast(id_holder<Config>* p) {
  return convertible_to<TargetType>(*p) ? static_cast<TargetType*>(p) : nullptr;
}
/**
 * @brief Приводит указатель на объект иерархии типов к указателю на целевой тип, если это возможно.
 *
 * @tparam TargetType Целевой тип.
 * @tparam Config Конфигурация типа.
 * @tparam T1 Дополнительный тип.
 * @param p Указатель на объект иерархии типов.
 * @return Указатель на объект целевого типа, если приведение возможно, иначе nullptr.
 */
template <typename TargetType, typename Config, typename T1>
constexpr TargetType* dyn_cast(id_holder<Config, T1, void>* p) {
  return convertible_to<TargetType>(*p) ? static_cast<TargetType*>(p) : nullptr;
}
/**
 * @brief Приводит указатель на константный объект иерархии типов к указателю на константный целевой тип, если это возможно.
 *
 * @tparam TargetType Константный целевой тип.
 * @tparam Config Конфигурация типа.
 * @param p Указатель на константный объект иерархии типов.
 * @return Указатель на константный объект целевого типа, если приведение возможно, иначе nullptr.
 */
template <typename TargetType, typename Config>
constexpr TargetType const* dyn_cast(id_holder<Config> const* p) {
  return convertible_to<TargetType>(*p) ? static_cast<TargetType*>(p) : nullptr;
}
/**
 * @brief Приводит указатель на константный объект иерархии типов к указателю на константный целевой тип, если это возможно.
 *
 * @tparam TargetType Константный целевой тип.
 * @tparam Config Конфигурация типа.
 * @tparam D Дополнительный тип.
 * @param p Указатель на константный объект иерархии типов.
 * @return Указатель на константный объект целевого типа, если приведение возможно, иначе nullptr.
 */
template <typename TargetType, typename Config, typename D>
constexpr TargetType const* dyn_cast(id_holder<Config, D, void> const* p) {
  return convertible_to<TargetType>(*p) ? static_cast<TargetType const*>(p)
                                        : nullptr;
}
/**
 * @brief Шаблонная структура для выбора типа целого числа в зависимости от количества битов.
 *
 * @tparam NumBits Количество битов.
 */
template <int NumBits>
struct int_for_bits {
  using type = typename int_for_bits<NumBits + 1>::type;
};
/**
 * @brief Специализация шаблонной структуры int_for_bits для 8 битов.
 */
template <>
struct int_for_bits<8> {
  using type = std::uint8_t;
};
/**
 * @brief Специализация шаблонной структуры int_for_bits для 16 битов.
 */
template <>
struct int_for_bits<16> {
  using type = std::uint16_t;
};
/**
 * @brief Специализация шаблонной структуры int_for_bits для 32 битов.
 */
template <>
struct int_for_bits<32> {
  using type = std::uint32_t;
};

/**
 * @brief Специализация шаблонной структуры int_for_bits для 64 битов.
 */
template <>
struct int_for_bits<64> {
  using type = std::uint64_t;
};

/**
 * @brief Структура для построения иерархии типов на основе заданной конфигурации.
 *
 * @tparam BaseType Базовый тип.
 * @tparam BitsPerLevel Количество битов на каждый уровень иерархии.
 */
template <typename BaseType, int... BitsPerLevel>
struct builder {
  /**
   * @brief Конфигурация для построения иерархии типов.
   */
  struct config {
    using base_type = BaseType;
    using bits_per_level = std::integer_sequence<int, BitsPerLevel...>;
    using level_shifts = meta::scan_add_t<int, 0, BitsPerLevel...>;
    using id_t =
        typename int_for_bits<meta::fold_add_v<int, BitsPerLevel...>>::type;
  };

  using root_t = type_hierarchy_detail::id_holder<config>;/**< Тип корня иерархии. */
};
/**
 * @brief Специализация шаблонной структуры builder для базового типа с установленными значениями по умолчанию для BitsPerLevel.
 */
template <typename BaseType>
struct builder<BaseType> : builder<BaseType, 8, 8, 8, 8> {};
/**
 * @brief Тип корня иерархии типов на основе заданной конфигурации.
 *
 * @tparam BaseType Базовый тип.
 * @tparam BitsPerLevel Количество битов на каждый уровень иерархии.
 */
template <typename BaseType, int... BitsPerLevel>
using root_t = typename builder<BaseType, BitsPerLevel...>::root_t;
/**
 * @brief Структура для определения подтипа производного типа от заданного супертипа.
 *
 * @tparam Derived Производный тип.
 * @tparam Super Супертип.
 */
template <typename Derived, typename Super>
struct sub_type_impl {
  using type = id_holder<get_config_t<Super>, Derived, Super>;
};
/**
 * @brief Специализация шаблонной структуры sub_type_impl для случая, когда супертип не задан.
 */
template <typename Derived, typename Config>
struct sub_type_impl<Derived, id_holder<Config>> {
  using type = id_holder<Config, Derived, void>;
};

}  // namespace type_hierarchy_detail

namespace type_hierarchy {
/**
 * @brief Создает иерархию типов от базового типа с заданным количеством уровней.
 *
 * @tparam BaseType Базовый тип.
 * @tparam BitsPerLevel Количество битов на каждый уровень иерархии.
 */
template <typename BaseType, int... BitsPerLevel>
using from_base = type_hierarchy_detail::root_t<BaseType, BitsPerLevel...>;
/**
 * @brief Определяет подтип для производного типа от заданного супертипа.
 *
 * @tparam Derived Производный тип.
 * @tparam Super Супертип.
 */
template <typename Derived, typename Super>
using sub_type =
    typename type_hierarchy_detail::sub_type_impl<Derived, Super>::type;

using type_hierarchy_detail::convertible_to;
}  // namespace type_hierarchy

using type_hierarchy::convertible_to;
using type_hierarchy::sub_type;

/**
 * @brief Макрос для определения подтипа.
 *
 * Определяет класс DERIVED как подтип класса SUPER.
 */
#define NI_SUB_TYPE(DERIVED, SUPER) \
  DERIVED : public                  \
  pm::sub_type<DERIVED, SUPER>

}  // namespace pm

#endif