#include <algorithm>
#include <array>

namespace cmplode
{

template<class IdList, class IdType>
constexpr auto compute_ode_index(const IdList& ids, IdType id)
{
  const auto posIt = std::find(std::begin(ids), std::end(ids), id);
  return std::distance(std::begin(ids), posIt);
}

template<class Idtype, std::size_t numberIds>
struct OdeIndexMap
{
  constexpr OdeIndexMap(std::array<Idtype, numberIds> usedIds_) :
    usedIds{ std::move(usedIds_) }
  {
  }

  std::array<Idtype, numberIds> usedIds;

  constexpr auto idx_of(Idtype wantedId) const
  {
    return compute_ode_index(usedIds, wantedId);
  }

  constexpr bool has_id(Idtype id) const
  {
    return std::find(std::begin(usedIds), std::end(usedIds), id) != std::end(usedIds);
  }
};

}