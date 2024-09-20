#include "rigid_geometric_algebra/detail/contract.hpp"
#include "skytest/skytest.hpp"

#include <sstream>
#include <type_traits>

auto main() -> int
{
  using namespace skytest::literals;
  using ::rigid_geometric_algebra::detail::contract_violation_handler;
  using ::rigid_geometric_algebra::detail::logging_violation_handler;
  using ::rigid_geometric_algebra::detail::postcondition;
  using ::rigid_geometric_algebra::detail::precondition;
  using ::skytest::aborts;
  using ::skytest::eq;
  using ::skytest::expect;
  using ::skytest::pred;

  "precondition with handler"_test = [] {
    return expect(aborts([] {
      precondition(false, contract_violation_handler{"{}", 42});
    }));
  };

  "precondition with logging"_test = [] {
    auto ss = std::stringstream{};

    precondition(false, logging_violation_handler{ss, "value is {}", 42});

    const auto contains = pred([&ss](auto substr) {
      return ss.view().contains(substr);
    });

    return expect(
        contains("PRECONDITION FAILURE") and
        contains("test/detail/contract_test.cpp:28") and contains("main") and
        contains("value is 42"));
  };

  "precondition with message"_test = [] {
    return expect(aborts([] { precondition(false, "failure"); }));
  };

  "without message"_test = [] {
    return expect(aborts([] { precondition(false); }));
  };

  "invocable in constant expressions"_test = [] {
    return expect([] {
      precondition(true);
      return true;
    }());
  };

  "cond is convertible to bool"_test = [] {
    return expect([] {
      precondition(std::bool_constant<true>{});
      return true;
    }());
  };

  "cond is convertible to bool"_test = [] {
    return expect([] {
      precondition(std::bool_constant<true>{});
      return true;
    }());
  };

  "postcondition with handler"_test = [] {
    return expect(aborts([] {
      postcondition(false, contract_violation_handler{"{}", 42});
    }));
  };

  "postcondition with logging"_test = [] {
    auto ss = std::stringstream{};

    postcondition(false, logging_violation_handler{ss, "value is {}", 42});

    const auto contains = pred([&ss](auto substr) {
      return ss.view().contains(substr);
    });

    return expect(
        contains("POSTCONDITION FAILURE") and
        contains("test/detail/contract_test.cpp:78") and contains("main") and
        contains("value is 42"));
  };

  "postcondition with message"_test = [] {
    return expect(aborts([] { postcondition(false, "failure"); }));
  };
}
