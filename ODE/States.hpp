#include <cstdint>
#include <tuple>

namespace cmplode
{

enum class state_ids {
  position_x = 0,
  velocity = 1,
  acceleration = 2
};

class position_x
{
public:
  constexpr static state_ids id = state_ids::position_x;

  template<class States>
  constexpr static double ode(const States& in)
  {
    return in.template value_of<state_ids::velocity>();
  }
};

class velocity
{
public:
  constexpr static state_ids id = state_ids::velocity;

  template<class States>
  constexpr static double acc_from(const States& in)
  {
    return in.template value_of<state_ids::acceleration>();
  }

  template<class States>
  constexpr static double ode(const States& in)
  {
    return acc_from(in);
  }
};

class acceleration
{
public:
  constexpr static state_ids id = state_ids::acceleration;

  template<class States>
  constexpr static double ode(const States& /*in*/)
  {
    return 3.0;
  }
};

}