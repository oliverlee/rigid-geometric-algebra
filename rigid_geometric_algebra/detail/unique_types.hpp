#pragma once

#include <tuple>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

template <class... Ts>
class unique_types_fn
{
  template <class T>
  struct get_fn
  {
    template <class... Us>
    constexpr static auto
    operator()(std::tuple<Us...> t) -> decltype(std::get<T>(t));
  };

public:
  constexpr static auto operator()() noexcept -> bool
  {
    return (std::is_invocable_v<get_fn<Ts>, std::tuple<Ts...>> and ...);
  }
};

template <class... Ts>
inline constexpr auto unique_types = unique_types_fn<Ts...>{};
}  // namespace rigid_geometric_algebra::detail
