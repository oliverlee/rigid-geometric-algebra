#pragma once

#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra::detail {

/// copy ref qualifiers from `From` to `To`
///
template <class From, class To>
struct copy_ref_qual
{
  using type = decltype(std::forward_like<From>(
      std::declval<std::remove_cvref_t<To>>()));
};

template <class From, class To>
using copy_ref_qual_t = typename copy_ref_qual<From, To>::type;

}  // namespace rigid_geometric_algebra::detail
