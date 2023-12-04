#include <array>
#include <tuple>
#include <iostream>

using id = cmplode::state_ids;

int main()
{
  constexpr std::tuple<cmplode::position_x, cmplode::velocity> diffGl;
  constexpr std::tuple<cmplode::acceleration> controlsTypes;

  constexpr cmplode::DynamicSystem<id, decltype(diffGl), decltype(controlsTypes)> dynamic(diffGl, controlsTypes);

  static_assert(dynamic.getNumberOfStates() == 2);
  static_assert(dynamic.getNumberOfControls() == 1);

  std::array states{ 0.0, 1.0 };
  constexpr std::array controls{ 2.0 };

  std::array statesOut{ 0.0, 1.0 };

  for (int iteration = 0; iteration < 1000; ++iteration) {

    dynamic.ode(statesOut, states, controls);

    std::transform(statesOut.begin(), statesOut.end(), states.begin(), states.begin(), [](const auto& stateDiff, const auto state) {
      return state + 1e-03 * stateDiff;
    });
  }

  std::copy(std::begin(states), std::end(states), std::ostream_iterator<double>(std::cout, " "));
}