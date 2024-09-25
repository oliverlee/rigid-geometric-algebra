#pragma once

#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// determine if one type is the same as another after applying `std::decay_t`
/// @tparam From type to decay
/// @tparam To type to compare against
///
/// @{

template <class From, class To>
struct decays_to : std::is_same<std::decay_t<From>, To>
{};

template <class From, class To>
inline constexpr auto decays_to_v = decays_to<From, To>::value;

/// @}

}  // namespace rigid_geometric_algebra::detail
