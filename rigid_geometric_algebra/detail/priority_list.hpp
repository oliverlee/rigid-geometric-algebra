#pragma once

#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <cstddef>
#include <functional>  // IWYU pragma: keep - std::minus<>
#include <utility>

namespace rigid_geometric_algebra::detail {

/// forward declarations
///
/// @{

template <class = void>
class derive_subtraction;

template <class = void, class...>
class derive_vector_space_operations;

/// @}

/// operation priority list
/// @tparam Op operation tag
///
/// Defines the priority list for synthesized overload sets with operation
/// `Op`. The primary template is defined to be an empty list. Specializations
/// must define a `type_list` value with types in priority order, where types
/// denote sources types that  provide a synthesized overload for `Op`. The
/// first type has the lowest priority.
///
/// For example, `derive_subtraction` and `derive_vector_space_operations`
/// both synthesize overloads associated with `std::minus<>`.
///
template <class Op>
inline constexpr auto priority_list = type_list<>{};

/// priority list for derived `std::minus<>` overloads
///
template <>
inline constexpr auto priority_list<std::minus<>> =
    type_list<derive_subtraction<>, derive_vector_space_operations<>>{};

/// returns the index of a type in a priority list
/// @tparam prio_list specialization of variable template `priority_list`
/// @tparam S type to determine the index of
///
template <auto prio_list, class S>
inline constexpr auto index_of = []<class... Ts>(type_list<Ts...>) {
  return []<std::size_t... Is>(std::index_sequence<Is...>) {
    return ((std::is_same_v<Ts, S> ? Is : 0UZ) + ...);
  }(std::index_sequence_for<Ts...>{});
}(prio_list);

/// returns the priority of a source type in a synthesized overload set
/// @tparam Op operation tag
/// @tparam S source type
///
/// Returns the priority of the overload synthesized by `S` in the synthesized
/// overload set denoted by `Op`.
///
template <class Op, class S>
inline constexpr auto priority_for = index_of<priority_list<Op>, S>;

/// returns the max priority in a synthesized overload set
/// @tparam Op operation tag
///
/// Returns the maximum priority of the synthesized overload set denoted by
/// `Op`. This is equivalent to the number of sources that can synthesize
/// overloads (as determined by the `priority_list<Op>`).
///
template <class Op>
inline constexpr auto max_priority = []<class... Ts>(type_list<Ts...>) {
  static_assert(sizeof...(Ts) != 0);
  return sizeof...(Ts) - 1;
}(priority_list<Op>);

}  // namespace rigid_geometric_algebra::detail
