#include <algorithm>
#include <cassert>
#include <utility>
#include <tuple>

namespace cmplode
{

namespace detail
{

template<typename>
struct array_size;
template<typename T, size_t N>
struct array_size<std::array<T, N>>
{
  static size_t const size = N;
};

template<class States, class StateMapper, std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
  ode([[maybe_unused]] const std::tuple<Tp...>& t, [[maybe_unused]] States& out, [[maybe_unused]] const StateMapper& in)
{}

template<class States, class StateMapper, std::size_t I = 0, typename... Tp>
  inline typename std::enable_if < I<sizeof...(Tp), void>::type
  ode(const std::tuple<Tp...>& t, States& out, const StateMapper& in)
{
  static_assert(I < std::tuple_size_v<States>);
  out[I] = std::get<I>(t).ode(in);
  ode<States, StateMapper, I + 1, Tp...>(t, out, in);
}

}// namespace detail

template<class IdType, class StateList, class ControlList>
class DynamicSystem
{
public:
  constexpr DynamicSystem(StateList states_, ControlList /*controlTypes_*/) :
    states(std::move(states_))
  {
  }

  constexpr auto getNumberOfStates() const
  {
    return std::tuple_size_v<StateList>;
  }

  constexpr auto getNumberOfControls() const
  {
    return std::tuple_size_v<ControlList>;
  }

  template<class States, class Controls>
  constexpr void ode(States& lhs, const States& stateValues, const Controls& controlValues) const
  {
    const StateAccessor<IdType, StateList, ControlList> stateMapper(stateValues, controlValues);
    detail::ode(states, lhs, stateMapper);
  }

private:
  StateList states;
};

}