#pragma once

#include <cstddef>

namespace rigid_geometric_algebra::detail {

/// priority dispatch tag
/// @tparam N priority
/// @see https://quuxplusone.github.io/blog/2021/07/09/priority-tag/
///
/// @{

template <std::size_t N>
struct priority : priority<N - 1>
{};

template <>
struct priority<0>
{};

/// @}

}  // namespace rigid_geometric_algebra::detail
