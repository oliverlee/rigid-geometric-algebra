#pragma once

namespace rigid_geometric_algebra::detail {

/// rebind template parameters from one type list to another
///
/// @{

template <class From, template <class...> class to>
struct rebind_args_into
{};

template <
    template <class...>
    class from,
    class... Ts,
    template <class...>
    class to>
struct rebind_args_into<from<Ts...>, to>
{
  using type = to<Ts...>;
};

template <class From, template <class...> class to>
using rebind_args_into_t = typename rebind_args_into<From, to>::type;

/// @}

}  // namespace rigid_geometric_algebra::detail
