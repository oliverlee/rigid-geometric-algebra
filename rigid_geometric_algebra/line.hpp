#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"
#include "rigid_geometric_algebra/detail/geometric_interface.hpp"
#include "rigid_geometric_algebra/glz_fwd.hpp"
#include "rigid_geometric_algebra/point.hpp"
#include "rigid_geometric_algebra/wedge.hpp"

#include <cstddef>
#include <format>
#include <numeric>
#include <ranges>
#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <class A>
  requires is_algebra_v<A>
using line_multivector_type_t = std::invoke_result_t<
    decltype(wedge),
    typename point<A>::multivector_type,
    typename point<A>::multivector_type>;

template <class D>
class check_invariant
{
  friend auto
  operator==(const check_invariant&, const check_invariant&) -> bool = default;

public:
  constexpr check_invariant() { static_cast<const D&>(*this).invariant(); }
};

}  // namespace detail

/// disable line invariant checks for a specific value type
///
/// Allow use of value types where the equality of zero and the inner product
/// of direction and moment may be indeterminable.
///
template <class>
inline constexpr auto disable_line_invariant = false;

template <class A>
  requires is_algebra_v<A>
class line
    : public detail::geometric_interface<detail::line_multivector_type_t<A>>,
      private detail::check_invariant<line<A>>
{
  static_assert(algebra_dimension_v<A> > 2);

  using base_type =
      detail::geometric_interface<detail::line_multivector_type_t<A>>;

public:
  /// algebra type
  ///
  using algebra_type = typename base_type::algebra_type;

  /// blade scalar type
  ///
  using value_type = typename base_type::value_type;

  /// multivector type
  ///
  using multivector_type = typename base_type::multivector_type;

private:
  friend detail::check_invariant<line>;

  constexpr auto invariant() const
  {
    const auto values = std::ranges::subrange(*this);

    const auto direction =
        std::views::take(values, multivector_type::size / 2U);
    const auto moment = std::views::drop(values, multivector_type::size / 2U);

    detail::invariant(
        disable_line_invariant<value_type> or
            value_type{} ==
                std::inner_product(
                    direction.begin(),
                    direction.end(),
                    moment.begin(),
                    value_type{}),
        detail::contract_violation_handler{
            "the `direction` and `moment` components of a line must be "
            "perpendicular:\ndirection: {}\nmoment: {}",
            direction,
            moment});
  }

public:
  /// default geometric type constructors
  ///
  using base_type::base_type;

  /// access the underlying `multivector`
  ///
  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  constexpr auto multivector(this Self&& self) noexcept -> std::conditional_t<
      std::is_lvalue_reference_v<Self&&>,
      const multivector_type&,
      const multivector_type&&>
  {
    using R = std::conditional_t<
        std::is_lvalue_reference_v<Self&&>,
        const multivector_type&,
        const multivector_type&&>;

    return static_cast<R>(self.base_type::multivector());
  }

  /// coefficient access
  ///
  /// @{

  constexpr auto begin() const noexcept -> base_type::const_iterator
  {
    return base_type::begin();
  }

  constexpr auto end() const noexcept -> base_type::const_iterator
  {
    return base_type::end();
  }

  /// @}

  /// equality comparison
  ///
  /// @{

  friend auto operator==(const line&, const line&) -> bool = default;

  /// @}
};

}  // namespace rigid_geometric_algebra

template <class A, class Char>
struct ::std::formatter<::rigid_geometric_algebra::line<A>, Char>
    : ::std::formatter<
          ::rigid_geometric_algebra::detail::geometric_interface<
              typename ::rigid_geometric_algebra::line<A>::multivector_type>,
          Char>
{};

template <class A>
struct ::glz::meta<::rigid_geometric_algebra::line<A>>
    : ::glz::meta<::rigid_geometric_algebra::detail::geometric_interface<
          typename ::rigid_geometric_algebra::line<A>::multivector_type>>
{};
