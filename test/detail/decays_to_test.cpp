#include "rigid_geometric_algebra/detail/decays_to.hpp"

struct T
{};

struct S : T
{};

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::decays_to;

  static_assert(decays_to<T, T>);
  static_assert(decays_to<const T, T>);
  static_assert(decays_to<T&, T>);
  static_assert(decays_to<const T&, T>);
  static_assert(decays_to<T&&, T>);
  static_assert(decays_to<const T&&, T>);

  static_assert(not decays_to<S, T>);
}
