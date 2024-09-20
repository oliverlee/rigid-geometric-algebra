#include "rigid_geometric_algebra/detail/size_checked_subrange.hpp"
#include "skytest/skytest.hpp"

#include <array>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::size_checked_subrange;

  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;

  static constexpr auto data = std::array{1, 2, 3};

  "size ok"_test = [] {
    return expect(eq(data[0], size_checked_subrange<3>(data)[0]));
  };

  "size not ok"_test = [] {
    return expect(aborts([] { return size_checked_subrange<2>(data); }));
  };
}
