#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

template <class T1, class T2>
static constexpr auto same = ::skytest::pred(std::is_same<T1, T2>{});

template <class Out, class In>
static constexpr auto test_blade_type_from = ::skytest::pred(
    std::is_same<
        Out,
        ::rigid_geometric_algebra::blade_type_from_mask_t<
            ::rigid_geometric_algebra::algebra_type_t<In>,
            In::dimension_mask>>{});

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;

  "blade type from ordering"_test = [] {
    return expect(
        test_blade_type_from<G2::blade<>, G2::blade<>>() and
        test_blade_type_from<G2::blade<0>, G2::blade<0>>() and
        test_blade_type_from<G2::blade<1>, G2::blade<1>>() and
        test_blade_type_from<G2::blade<2>, G2::blade<2>>() and
        test_blade_type_from<G2::blade<0, 1, 2>, G2::blade<0, 1, 2>>());
  };

  "blade type from ordering is canonical"_test = [] {
    return expect(
        test_blade_type_from<G2::blade<0, 1>, G2::blade<0, 1>>() and
        test_blade_type_from<G2::blade<0, 1>, G2::blade<1, 0>>() and
        test_blade_type_from<G2::blade<0, 1, 2>, G2::blade<2, 1, 0>>() and
        test_blade_type_from<G2::blade<0, 1, 2>, G2::blade<1, 0, 2>>());
  };

  "blade type from mask"_test = [] {
    using ::rigid_geometric_algebra::blade_type_from_mask_t;

    return expect(
        same<G2::blade<0, 1, 2>, blade_type_from_mask_t<G2, 0b111>>() and
        (not same<G2::blade<2, 1, 0>, blade_type_from_mask_t<G2, 0b111>>()) and
        same<G2::blade<0, 1>, blade_type_from_mask_t<G2, 0b011>>() and
        same<G2::blade<0, 1>, blade_type_from_mask_t<G2, 0b11>>() and
        same<G2::blade<1, 2>, blade_type_from_mask_t<G2, 0b110>>());
  };

  "blade type from dimensions"_test = [] {
    using ::rigid_geometric_algebra::blade_type_from_dimensions_t;

    return expect(
        same<G2::blade<>, blade_type_from_dimensions_t<G2, {}>>() and
        same<G2::blade<0>, blade_type_from_dimensions_t<G2, {0}>>() and
        same<G2::blade<1, 2>, blade_type_from_dimensions_t<G2, {1, 2}>>() and
        same<G2::blade<2, 1, 0>,
             blade_type_from_dimensions_t<G2, {2, 1, 0}>>());
  };
}
