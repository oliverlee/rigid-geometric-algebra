#pragma once

#include "rigid_geometric_algebra/detail/indices_array.hpp"

#include <array>
#include <cstddef>
#include <tuple>
#include <utility>
namespace rigid_geometric_algebra::detail {

/// filter types in a type list
///
/// Defines static member typedef `type` as a specialization of L with type
/// `T` in `Ts...` if `pred.operator()<T>()` is `true`.
///
/// @{

template <class L, auto pred>
struct type_filter
{};

template <template <class...> class list, class... Ts, auto pred>
struct type_filter<list<Ts...>, pred>
{
  static constexpr auto indices = detail::indices_array<
      std::array<bool, sizeof...(Ts)>{pred.template operator()<Ts>()...}>;

  using type =
      decltype([]<std::size_t... Is>(std::index_sequence<Is...>)
                   -> list<std::tuple_element_t<
                       indices[Is],
                       std::tuple<Ts...>>...> {
        return {};
      }(std::make_index_sequence<indices.size()>{}));
};

template <class L, auto pred>
using type_filter_t = typename type_filter<L, pred>::type;

/// @}

}  // namespace rigid_geometric_algebra::detail
