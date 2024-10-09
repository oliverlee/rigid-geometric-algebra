#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"
#include "rigid_geometric_algebra/detail/counted_sort.hpp"
#include "rigid_geometric_algebra/detail/even.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>

namespace rigid_geometric_algebra {
namespace detail {

template <std::size_t N>
class canonical_dimension_order_fn
{
public:
  template <std::ranges::random_access_range R>
    requires (std::sortable<std::ranges::iterator_t<R>> and
              std::same_as<std::size_t, std::ranges::range_value_t<R>>)
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  static constexpr auto operator()(R&& dimensions) -> void
  {
    detail::precondition(dimensions.size() <= N);

    std::ranges::sort(dimensions);

    // if all dimensions are present, return lexicographic order
    if (dimensions.size() == N) {
      return;
    }

    // skip the first dimension if it is 0
    const auto first_is_zero =
        not dimensions.empty() and dimensions.front() == 0;
    const auto bulk =
        std::ranges::subrange(dimensions) |
        std::views::drop(int{first_is_zero});

    // partition all dimensions to
    // [ bulk | complement(bulk) ]
    // e.g. N = 4
    // [ 2, 3 | 0, 1 ]
    //  or
    // [ 1, 3 | 0, 2 ]
    auto partitioned = std::array<std::size_t, N>{};
    const auto [_, it] = std::ranges::copy(bulk, partitioned.begin());
    std::ranges::set_difference(std::views::iota(0UZ, N), bulk, it);

    // If sorting [ bulk | complement(bulk) ] requires an odd number of
    // swaps, reverse the bulk part of dimensions (0 remains the first element
    // if present). Otherwise, return dimensions (sorted to be in
    // lexicographic order).
    if (not detail::even(detail::counted_sort(partitioned))) {
      std::ranges::reverse(bulk);
    }
  }
};

}  // namespace detail

/// reorders a range of dimensions to canonical order
/// @tparam N algebra dimension
///
/// Reorders dimensions to canonical order. For grade 2 blades without a
/// 0-dimensional basis, canonical order is equivalent to cyclic order:
///
/// ```
/// (1, 2) -> (1, 2)
/// (1, 3) -> (3, 1)
/// (2, 3) -> (2, 3)
/// ```
///
/// This reduces the need to negate blades when performing geometric operations.
///
template <std::size_t N>
inline constexpr auto canonical_dimension_order =
    detail::canonical_dimension_order_fn<N>{};
}  // namespace rigid_geometric_algebra
