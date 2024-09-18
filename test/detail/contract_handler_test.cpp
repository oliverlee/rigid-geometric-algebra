#include "rigid_geometric_algebra/detail/contract.hpp"

#include <cstdlib>

auto main() -> int
{
  using ::rigid_geometric_algebra::detail::contract_violation_handler;
  using ::rigid_geometric_algebra::detail::precondition;

  precondition(false, contract_violation_handler{"value is {}", 42});
}
