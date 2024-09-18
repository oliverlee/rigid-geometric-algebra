#pragma once

#include <cstddef>
namespace rigid_geometric_algebra::detail {

inline constexpr auto even = [](std::size_t value) {
  return value % 2UZ == 0UZ;
};
}  // namespace rigid_geometric_algebra::detail
