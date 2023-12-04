#include <algorithm>
#include <cassert>
#include <tuple>
#include <span>

namespace cmplode
{

template<int... Indices>
struct indices
{
  using next = indices<Indices..., static_cast<int>(sizeof...(Indices))>;
};

template<int Size>
struct build_indices
{
  using type = typename build_indices<Size - 1>::type::next;
};

template<>
struct build_indices<0>
{
  using type = indices<>;
};

template<typename T>
using Bare = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template<typename Tuple>
constexpr
  typename build_indices<std::tuple_size<Bare<Tuple>>::value>::type
  make_indices()
{
  return {};
}

template<typename Tuple, int... Indices>
constexpr std::array<
  typename std::tuple_element<0, Bare<Tuple>>::type,
  std::tuple_size<Bare<Tuple>>::value>
  to_array(Tuple&& tuple, indices<Indices...>)
{
  using std::get;
  return { { get<Indices>(std::forward<Tuple>(tuple))... } };
}

template<typename Tuple>
constexpr auto to_array(Tuple&& tuple)
  -> decltype(to_array(std::declval<Tuple>(), make_indices<Tuple>()))
{
  return to_array(std::forward<Tuple>(tuple), make_indices<Tuple>());
}

template<typename... Ts, typename Function, size_t... Is>
constexpr auto transform_impl(std::tuple<Ts...> const& inputs, Function function, std::index_sequence<Is...>)
{
  return std::tuple<std::result_of_t<Function(Ts)>...>{ function(std::get<Is>(inputs))... };
}

template<typename... Ts, typename Function>
constexpr auto transform(std::tuple<Ts...> const& inputs, Function function)
{
  return transform_impl(inputs, function, std::make_index_sequence<sizeof...(Ts)>{});
}

template<class StateList>
constexpr auto create_id_array(const StateList& usedStates)
{
  return to_array(transform(usedStates, [](const auto state) { return state.id; }));
}

template<class IdType, class StateList, class ControlList>
class StateAccessor
{
public:
  template<class States, class Controls>
  constexpr StateAccessor(const States& states, const Controls& controls) :
    state_values{ std::span(states.data(), static_cast<std::span<const double>::size_type>(states.size())) },
    control_values{ std::span(controls.data(), static_cast<std::span<const double>::size_type>(controls.size())) }
  {
  }

  template<IdType id>
  constexpr double value_of() const
  {
    constexpr OdeIndexMap<IdType, std::tuple_size_v<StateList>> id_to_idx_states(create_id_array(StateList()));

    constexpr OdeIndexMap<IdType, std::tuple_size_v<ControlList>> id_to_idx_controls(create_id_array(ControlList()));

    if constexpr (id_to_idx_states.has_id(id)) {
      return state_values[id_to_idx_states.idx_of(id)];
    }

    if constexpr (id_to_idx_controls.has_id(id)) {
      return control_values[id_to_idx_controls.idx_of(id)];
    }

    assert(false && "Unkown Id");
    return 0.0;
  }

private:
  std::span<const double> state_values{};
  std::span<const double> control_values{};
};

}