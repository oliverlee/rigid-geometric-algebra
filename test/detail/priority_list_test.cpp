#include "rigid_geometric_algebra/detail/priority_list.hpp"
#include "skytest/skytest.hpp"

#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;

  using ::rigid_geometric_algebra::detail::index_of_fn;
  using ::rigid_geometric_algebra::detail::max_priority_fn;
  using ::rigid_geometric_algebra::detail::type_list;

  static constexpr auto prio_list = type_list<char, int, float>{};

  "index_of"_test = [] {
    return expect(
        eq(0, index_of_fn(prio_list, std::type_identity<char>{})) and
        eq(1, index_of_fn(prio_list, std::type_identity<int>{})) and
        eq(2, index_of_fn(prio_list, std::type_identity<float>{})));
  };

  "index_of pre"_test = [] {
    return expect(aborts([] {
      return index_of_fn(prio_list, std::type_identity<double>{});
    }));
  };

  "max_priority"_test = [] {
    return expect(eq(2, max_priority_fn(prio_list)));
  };

  "max_priority pre"_test = [] {
    return expect(aborts([] { return max_priority_fn(type_list{}); }));
  };
}
