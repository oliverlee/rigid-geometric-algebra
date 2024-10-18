#pragma once

/// An algebra that unifies geometry, algebra, and transformations
///
/// Rigid geometric algebra is a mathematical model that naturally incorporates
/// representations for Euclidean points, lines, and planes in N-D space as well
/// as operations for performing rotations, reflections, and translations in a
/// single algebraic structure.
///
namespace rigid_geometric_algebra {}  // namespace rigid_geometric_algebra

// IWYU pragma: begin_exports
#include "rigid_geometric_algebra/algebra.hpp"
#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_fwd.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/antiwedge.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/blade_complement_type.hpp"
#include "rigid_geometric_algebra/blade_dimensions.hpp"
#include "rigid_geometric_algebra/blade_sum.hpp"
#include "rigid_geometric_algebra/blade_type_from.hpp"
#include "rigid_geometric_algebra/canonical_dimension_order.hpp"
#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/complement.hpp"
#include "rigid_geometric_algebra/field.hpp"
#include "rigid_geometric_algebra/field_identity.hpp"
#include "rigid_geometric_algebra/get.hpp"
#include "rigid_geometric_algebra/get_or.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"
#include "rigid_geometric_algebra/join.hpp"
#include "rigid_geometric_algebra/line.hpp"
#include "rigid_geometric_algebra/magma.hpp"
#include "rigid_geometric_algebra/meet.hpp"
#include "rigid_geometric_algebra/multivector.hpp"
#include "rigid_geometric_algebra/one.hpp"
#include "rigid_geometric_algebra/plane.hpp"
#include "rigid_geometric_algebra/point.hpp"
#include "rigid_geometric_algebra/scalar_type.hpp"
#include "rigid_geometric_algebra/to_multivector.hpp"
#include "rigid_geometric_algebra/unit_hypervolume.hpp"
#include "rigid_geometric_algebra/wedge.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"
// IWYU pragma: end_exports
