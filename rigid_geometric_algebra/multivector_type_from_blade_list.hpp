#pragma once

#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/multivector.hpp"

namespace rigid_geometric_algebra {

/// obtain the multivector type from a list of blades
///
/// @{

template <class>
struct multivector_type_from_blade_list
{};

template <template <class...> class list, class... Bs>
struct multivector_type_from_blade_list<list<Bs...>>
{
  using type = multivector<common_algebra_type_t<Bs...>, Bs...>;
};

template <class BladeList>
using multivector_type_from_blade_list_t =
    typename multivector_type_from_blade_list<BladeList>::type;

/// @}

}  // namespace rigid_geometric_algebra
