#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
  using ::rigid_geometric_algebra::blade_ordering;
  using ::rigid_geometric_algebra::blade_type_from_t;

  "blade type from ordering"_test = [] {
    using B = blade_type_from_t<blade_ordering<G2>{
        std::type_identity<G2::blade<>>{}}>;

    using B0 = blade_type_from_t<blade_ordering<G2>{
        std::type_identity<G2::blade<0>>{}}>;
    using B1 = blade_type_from_t<blade_ordering<G2>{
        std::type_identity<G2::blade<1>>{}}>;
    using B2 = blade_type_from_t<blade_ordering<G2>{
        std::type_identity<G2::blade<2>>{}}>;

    using B012 = blade_type_from_t<blade_ordering<G2>{
        std::type_identity<G2::blade<0, 1, 2>>{}}>;

    return expect(
        std::is_same_v<G2::blade<>, B> and std::is_same_v<G2::blade<0>, B0> and
        std::is_same_v<G2::blade<1>, B1> and
        std::is_same_v<G2::blade<2>, B2> and
        std::is_same_v<G2::blade<0, 1, 2>, B012>);
  };

  "blade type from ordering is canonical"_test = [] {
    using B01 = blade_type_from_t<blade_ordering<G2>{
        std::type_identity<G2::blade<0, 1>>{}}>;
    using B10 = blade_type_from_t<blade_ordering<G2>{
        std::type_identity<G2::blade<1, 0>>{}}>;

    return expect(
        std::is_same_v<G2::blade<0, 1>, B01> and
        std::is_same_v<G2::blade<0, 1>, B10>);
  };
}
