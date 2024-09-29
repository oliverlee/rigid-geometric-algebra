#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ranges>

namespace rigid_geometric_algebra::detail {

/// determine indices of a range that satisfy a unary predicate
///
inline constexpr class
{
public:
  template <
      std::ranges::forward_range R,
      std::weakly_incrementable O,
      class UnaryPredicate = std::identity>
    requires std::unsigned_integral<std::iter_value_t<O>> and
             std::indirect_unary_predicate<
                 UnaryPredicate,
                 std::ranges::iterator_t<R>>
  static constexpr auto
  operator()(R&& rng, O dest, UnaryPredicate pred = {}) -> O
  {
    for (auto index = std::iter_value_t<O>{};
         const auto& element : std::forward<R>(rng)) {
      if (pred(element)) {
        *dest++ = index;
      }
      ++index;
    }
    return dest;
  }
} indices_of{};

}  // namespace rigid_geometric_algebra::detail
