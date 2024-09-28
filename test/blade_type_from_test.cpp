#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

template <class Out, class In>
struct impl
{
  constexpr operator bool() const
  {
    using ::rigid_geometric_algebra::algebra_type_t;
    using ::rigid_geometric_algebra::blade_ordering;
    using ::rigid_geometric_algebra::blade_type_from_t;

    return Out{} == blade_type_from_t<algebra_type_t<In>, In::dimension_mask>{};
  }
};

template <class Out, class In>
static constexpr auto test_blade_type_from = impl<Out, In>{};

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;

  "blade type from ordering"_test = [] {
    return expect(
        test_blade_type_from<G2::blade<>, G2::blade<>> and
        test_blade_type_from<G2::blade<0>, G2::blade<0>> and
        test_blade_type_from<G2::blade<1>, G2::blade<1>> and
        test_blade_type_from<G2::blade<2>, G2::blade<2>> and
        test_blade_type_from<G2::blade<0, 1, 2>, G2::blade<0, 1, 2>>);
  };

  "blade type from ordering is canonical"_test = [] {
    return expect(
        test_blade_type_from<G2::blade<0, 1>, G2::blade<0, 1>> and
        test_blade_type_from<G2::blade<0, 1>, G2::blade<1, 0>> and
        test_blade_type_from<G2::blade<0, 1, 2>, G2::blade<2, 1, 0>> and
        test_blade_type_from<G2::blade<0, 1, 2>, G2::blade<1, 0, 2>>);
  };
}
