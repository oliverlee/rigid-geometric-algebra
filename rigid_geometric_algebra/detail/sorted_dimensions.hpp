#pragma once

#include "rigid_geometric_algebra/detail/unique_dimensions.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

inline constexpr class
{
  template <std::size_t N>
  using constant = std::integral_constant<std::size_t, N>;

  template <std::size_t N, class... Is>
  static constexpr auto make_sequence_from(Is... is)
  {
    if constexpr (((N > Is::value) and ...)) {
      return std::tuple{};
    } else if constexpr (((N == Is::value) or ...)) {
      return std::tuple_cat(
          std::tuple<constant<N>>{}, make_sequence_from<N + 1>(is...));
    } else {
      return make_sequence_from<N + 1>(is...);
    }
  }

public:
  constexpr static auto operator()() { return std::index_sequence<>{}; }

  template <class... Is>
  constexpr static auto operator()(Is... is)
  {
    return []<class... Js>(
               std::tuple<Js...>) -> std::index_sequence<Js::value...> {
      return {};
    }(make_sequence_from<0>(is...));
  }

} sort_dimensions_impl{};

template <std::size_t... Is>
  requires (unique_dimensions(Is...))
struct sorted_dimensions
    : decltype(sort_dimensions_impl(
          std::integral_constant<std::size_t, Is>{}...)){};

}  // namespace rigid_geometric_algebra::detail
