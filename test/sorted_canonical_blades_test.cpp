#include "rigid_geometric_algebra/rigid_geometric_algebra.hpp"

#include <format>
#include <iterator>
#include <ostream>
#include <vector>

namespace {

// skytest doesn't handle printing ranges-of-ranges
template <class T>
auto operator<<(std::ostream& os, const std::vector<T>& v) -> std::ostream&
{
  auto out = std::ostream_iterator<char>(os);
  std::format_to(out, "{}", v);
  return os;
}

}  // namespace

#include "skytest/skytest.hpp"

#include <algorithm>
#include <ranges>
#include <type_traits>

using G2 = ::rigid_geometric_algebra::algebra<double, 2>;
using G3 = ::rigid_geometric_algebra::algebra<double, 3>;
using ::rigid_geometric_algebra::sorted_canonical_blades_t;
using ::rigid_geometric_algebra::detail::type_list;

template <class T1, class T2>
static constexpr auto same = ::skytest::pred(std::is_same<T1, T2>{});

static constexpr auto to_range = []<class... Bs>(type_list<Bs...>) {
  return std::vector{std::vector{std::from_range, Bs::dimensions}...};
};

template <class T1, class T2>
static constexpr auto equal = [] {
  return ::skytest::pred(std::ranges::equal)(to_range(T1{}), to_range(T2{}));
};

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::eq;
  using ::skytest::expect;

  "not in order"_test = [] {
    return expect(
        same<
            type_list<
                G2::blade<>,
                G2::blade<0>,
                G2::blade<1>,
                G2::blade<2>,
                G2::blade<0, 1>,
                G2::blade<0, 2>,
                G2::blade<1, 2>,
                G2::blade<0, 1, 2>>,
            sorted_canonical_blades_t<
                G2::blade<0>,
                G2::blade<>,
                G2::blade<1>,
                G2::blade<2>,
                G2::blade<0, 1>,
                G2::blade<0, 2>,
                G2::blade<1, 2>,
                G2::blade<0, 1, 2>>>());
  };

  "point blade order"_test = [] {
    return expect(
        same<type_list<G3::blade<0>, G3::blade<1>, G3::blade<2>, G3::blade<3>>,
             sorted_canonical_blades_t<
                 G3::blade<0>,
                 G3::blade<1>,
                 G3::blade<2>,
                 G3::blade<3>>>());
  };

  "line blade order"_test = [] {
    return expect(
        equal<
            type_list<
                G3::blade<0, 1>,
                G3::blade<0, 2>,
                G3::blade<0, 3>,
                G3::blade<2, 3>,
                G3::blade<1, 3>,  // TODO fix basis order
                G3::blade<1, 2>>,
            sorted_canonical_blades_t<
                G3::blade<0, 1>,
                G3::blade<0, 2>,
                G3::blade<0, 3>,
                G3::blade<1, 2>,
                G3::blade<3, 1>,
                G3::blade<2, 3>>>());
  };

  "plane blade order"_test = [] {
    return expect(
        equal<
            type_list<
                G3::blade<0, 2, 3>,
                G3::blade<0, 1, 3>,  // TODO fix basis order
                G3::blade<0, 1, 2>,
                G3::blade<1, 2, 3>>,  // TODO fix basis order
            sorted_canonical_blades_t<
                G3::blade<0, 2, 3>,
                G3::blade<0, 3, 1>,
                G3::blade<0, 1, 2>,
                G3::blade<3, 2, 1>>>());
  };

  "has duplicates"_test = [] {
    return expect(
        same<type_list<G2::blade<>, G2::blade<0>, G2::blade<1>, G2::blade<2>>,
             sorted_canonical_blades_t<
                 G2::blade<0>,
                 G2::blade<>,
                 G2::blade<>,
                 G2::blade<1>,
                 G2::blade<2>>>());
  };
}
