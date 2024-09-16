#include "rigid_geometric_algebra/detail/tuple_cref.hpp"
#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using ::rigid_geometric_algebra::multivector;
  using ::rigid_geometric_algebra::detail::tuple_cref;

  static constexpr auto v = multivector{G2::blade<1, 2>{1}};

  "invoke tuple cref on multivector"_test = [] {
    return expect(eq(G2::multivector<G2::blade<1, 2>>{1}, tuple_cref(v)));
  };

  "not invocable on rvalues"_test = [] {
    using V = std::remove_cvref_t<decltype(v)>;

    static_assert(not std::is_invocable_v<decltype(tuple_cref), V&&>);
    static_assert(not std::is_invocable_v<decltype(tuple_cref), const V&&>);

    return expect(true);
  };
}
