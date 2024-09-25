#include "rigid_geometric_algebra/detail/decays_to.hpp"

struct T
{};

struct S : T
{};

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::decays_to_v;

  static_assert(decays_to_v<T, T>);
  static_assert(decays_to_v<const T, T>);
  static_assert(decays_to_v<T&, T>);
  static_assert(decays_to_v<const T&, T>);
  static_assert(decays_to_v<T&&, T>);
  static_assert(decays_to_v<const T&&, T>);

  static_assert(not decays_to_v<S, T>);
}
