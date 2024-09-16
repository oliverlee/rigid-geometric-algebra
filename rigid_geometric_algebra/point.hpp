#pragma once

#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/multivector.hpp"

#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {

template <class A>
  requires is_algebra_v<A>
class point
{
  using multivector_type_ =
      decltype([]<std::size_t... Is>(std::index_sequence<Is...>)
                   -> multivector<A, blade<A, Is>...> {
        return {};
      }(std::make_index_sequence<algebra_dimension_v<A>>{}));

  multivector_type_ values_{};

public:
  /// algebra type
  ///
  using algebra_type = A;

  /// blade scalar type
  ///
  using value_type = algebra_field_t<A>;

  /// multivector_type
  ///
  using multivector_type = multivector_type_;

  /// construct a zero point
  ///
  point() = default;

  /// construct a point, specifying the coefficients
  /// @tparam Ts types `value_type` is constructible from
  /// @tparam coeffs coefficient values
  ///
  template <class... Ts>
    requires (sizeof...(Ts) == multivector_type::size()) and
             (std::constructible_from<value_type, Ts> and ...)
  constexpr point(Ts&&... coeffs) : values_{std::forward<Ts>(coeffs)...}
  {}

  /// initializer list constructor
  /// @param il initializer list of values
  ///
  /// @note allows conversions to `value_type` is floating point and if the
  ///   source can be stored exactly
  /// @see
  /// https://en.cppreference.com/w/cpp/language/list_initialization#Narrowing_conversions
  ///
  constexpr point(std::initializer_list<value_type> il)
    requires std::floating_point<value_type>
      : values_{il}
  {}

  /// access the underlying `multivector`
  ///
  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward): see forward_like
  constexpr auto multivector(this Self&& self) noexcept
      -> decltype(std::forward_like<Self>(self.values_))
  {
    return std::forward_like<Self>(self.values_);
  }

  /// coefficient access
  /// @tparam Self `this` type
  /// @param self explicit `this` parameter
  /// @param i coefficient to access
  ///
  /// returns `get<blade<algebra_type, i>>(point.multivector())`
  ///
  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward): see forward_like
  constexpr auto operator[](this Self&& self, std::size_t i)
      -> decltype(std::forward_like<Self>(std::declval<value_type>()))
  {
    auto refs = [&self]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array{std::ref(
          get<blade<algebra_type, Is>>(self.multivector()).coefficient)...};
    }(std::make_index_sequence<multivector_type::size()>{});

    return std::forward_like<Self>(refs[i].get());
  }

  /// wedge product
  ///
  /// @{

  template <class P>
    requires std::is_same_v<P, point>
  friend constexpr auto operator^(const P& p, const P& q)
      // TODO return line
      -> decltype(p.multivector() ^ q.multivector())
  {
    return p.multivector() ^ q.multivector();
  }

  /// @}

  /// equality comparison
  ///
  /// @{

  friend auto operator==(const point&, const point&) -> bool = default;

  /// @}
};

}  // namespace rigid_geometric_algebra

template <class A, class CharT>
struct std::formatter<::rigid_geometric_algebra::point<A>, CharT>
    : std::formatter<
          typename ::rigid_geometric_algebra::point<A>::multivector_type,
          CharT>
{
  // https://github.com/llvm/llvm-project/issues/66466
  template <class O>
  constexpr auto
  format(const ::rigid_geometric_algebra::point<A>& p,
         std::basic_format_context<O, CharT>& ctx) const
  {
    return std::formatter<
        typename ::rigid_geometric_algebra::point<A>::multivector_type,
        CharT>::format(p.multivector(), ctx);
  }
};
