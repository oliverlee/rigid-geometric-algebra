#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/geometric_fwd.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"

#include <concepts>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// function object to convert a `multivector` to a geometric type
///
template <template <class> class to>
class to_geometric_type_fn
{
public:
  template <detail::multivector V>
    requires std::constructible_from<to<algebra_type_t<V>>, V>
  static constexpr auto operator()(V&& v) -> to<algebra_type_t<V>>
  {
    return to<algebra_type_t<V>>{std::forward<V>(v)};
  }
};

/// function object to convert a `multivector` to one of multiple geometric
/// types
///
/// @{

template <template <class> class... to>
class to_geometric_fn : public to_geometric_type_fn<to>...
{
public:
  using to_geometric_type_fn<to>::operator()...;
};

inline constexpr auto to_geometric = to_geometric_fn<point, line, plane>{};

/// @}

/// defines a geometric overload from `multivector` overloads
///
class geometric_operator
{
public:
  template <class Self, detail::geometric... Ts>
    requires std::is_invocable_v<
        decltype(to_geometric),
        std::invoke_result_t<
            const Self&,
            decltype(std::declval<Ts>().multivector())...>>
  constexpr auto operator()(this const Self& self, Ts&&... ts)
      -> decltype(to_geometric(self(std::forward<Ts>(ts).multivector()...)))
  {
    return to_geometric(self(std::forward<Ts>(ts).multivector()...));
  }
};

}  // namespace rigid_geometric_algebra::detail
