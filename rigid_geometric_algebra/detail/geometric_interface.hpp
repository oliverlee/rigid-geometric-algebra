#pragma once

#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/glz_fwd.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"

#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <initializer_list>
#include <utility>

namespace rigid_geometric_algebra::detail {

/// Base type helper for geometric types
/// @tparam V multivector type
///
/// Define a multivector data member and provides definitions for members common
/// to geometric types.
///
template <detail::multivector V>
class geometric_interface
{
  V values_;

public:
  /// geometric interface type
  ///
  using geometric_interface_type = geometric_interface;

  /// algebra type
  ///
  using algebra_type = algebra_type_t<V>;

  /// blade scalar type
  ///
  using value_type = algebra_field_t<algebra_type>;

  /// multivector type
  ///
  using multivector_type = V;

  /// construct a zero geometric type
  ///
  geometric_interface() = default;

  /// construct a geometric type, specifying the coefficients
  /// @tparam Ts parameter pack of types
  /// @param coeffs coefficient values
  ///
  template <class... Ts>
    requires (sizeof...(Ts) == multivector_type::size()) and
             (std::constructible_from<value_type, Ts> and ...)
  constexpr geometric_interface(Ts&&... coeffs)
      : values_{std::forward<Ts>(coeffs)...}
  {}

  /// initializer list constructor
  /// @param il initializer list of values
  ///
  /// @note allows conversions to `value_type` is floating point and if the
  ///   source can be stored exactly
  /// @see
  /// https://en.cppreference.com/w/cpp/language/list_initialization#Narrowing_conversions
  /// @pre `il.size() == multivector::size` is `true`
  ///
  constexpr geometric_interface(std::initializer_list<value_type> il)
    requires std::floating_point<value_type>
      : values_{il}
  {}

  /// access the underlying `multivector`
  ///
  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  constexpr auto multivector(this Self&& self) noexcept
      -> decltype(std::forward_like<Self&&>(self.values_))
  {
    return std::forward_like<Self&&>(self.values_);
  }

  /// coefficient access
  /// @tparam Self `this` type
  /// @param self explicit `this` parameter
  /// @param i coefficient to access
  ///
  /// returns `get<blade<algebra_type, i>>(point.multivector())`
  /// (reference to `scalar_type`)
  ///
  template <class Self>
  // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
  constexpr auto operator[](this Self&& self, std::size_t i)
      -> decltype(std::forward_like<Self>(std::declval<value_type>()))
  {
    auto refs = [&self]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array{
          std::ref(self.multivector().template get<Is>().coefficient)...};
    }(std::make_index_sequence<multivector_type::size>{});

    return std::forward_like<Self>(refs[i].get());
  }

  /// equality comparison
  ///
  /// @{

  friend auto operator==(const geometric_interface&, const geometric_interface&)
      -> bool = default;

  /// @}
};

}  // namespace rigid_geometric_algebra::detail

template <class V, class Char>
struct ::std::
    formatter<::rigid_geometric_algebra::detail::geometric_interface<V>, Char>
    : ::std::formatter<
          typename ::rigid_geometric_algebra::detail::geometric_interface<
              V>::multivector_type,
          Char>
{
  // https://github.com/llvm/llvm-project/issues/66466
  template <class O>
  constexpr auto
  format(const ::rigid_geometric_algebra::detail::geometric_interface<V>& p,
         ::std::basic_format_context<O, Char>& ctx) const
  {
    return ::std::formatter<
        typename ::rigid_geometric_algebra::detail::geometric_interface<
            V>::multivector_type,
        Char>::format(p.multivector(), ctx);
  }
};

template <class V>
struct ::glz::meta<::rigid_geometric_algebra::detail::geometric_interface<V>>
{
  template <class P>
  struct geometric_wrapper
  {
    std::reference_wrapper<P> point;
  };

  template <class T>
  geometric_wrapper(T&) -> geometric_wrapper<T>;

  static constexpr auto value = [](auto& self) {
    return geometric_wrapper{self.multivector().as_tuple()};
  };
};
