#pragma once

#include <utility>

namespace rigid_geometric_algebra::detail {

auto to_multivector() -> void;

/// implementation-only concept to determine if a type is promotable to a
/// `multivector`
///
template <class T>
concept multivector_promotable = requires {
  to_multivector(std::declval<T>());
};

}  // namespace rigid_geometric_algebra::detail
