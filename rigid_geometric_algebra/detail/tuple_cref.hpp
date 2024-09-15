#pragma once

#include <tuple>

namespace rigid_geometric_algebra::detail {

inline constexpr class
{
public:
  template <class T>
  static constexpr auto operator()(const T&&) noexcept = delete;

  template <class... Ts>
  static constexpr auto operator()(const std::tuple<Ts...>& value) noexcept
      -> const std::tuple<Ts...>&
  {
    return value;
  }
} tuple_cref{};

}  // namespace rigid_geometric_algebra::detail
