#pragma once

#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// determine if one type is the same as another after applying `std::decay_t`
/// @tparam From type to decay
/// @tparam To type to compare against
///
/// @{

template <class From, class To>
concept decays_to = std::is_same_v<std::decay_t<From>, To>;

/// @}

}  // namespace rigid_geometric_algebra::detail
