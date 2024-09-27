#pragma once

#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <cstddef>
#include <tuple>
#include <utility>

namespace rigid_geometric_algebra::detail {

template <
    std::size_t I,
    class Tuple1,
    class Tuple2,
    std::size_t M = std::tuple_size_v<Tuple2>>
constexpr auto ith_pair(const Tuple1& tuple1, const Tuple2& tuple2)
    -> std::pair<std::tuple_element_t<I / M, Tuple1>,
                 std::tuple_element_t<I % M, Tuple2>>;

/// defines the cartesian product of two type lists
///
/// the resulting elements are specializations of `std::pair`
///
/// @{

template <class T1, class T2>
struct type_product
{};

template <
    template <class...>
    class list1,
    template <class...>
    class list2,
    class... T1s,
    class... T2s>
struct type_product<list1<T1s...>, list2<T2s...>>
{
  template <std::size_t... Is>
  static constexpr auto impl(std::index_sequence<Is...>)
      -> detail::type_list<decltype(ith_pair<Is>(
          std::declval<std::tuple<T1s...>>(),
          std::declval<std::tuple<T2s...>>()))...>;

  using type = decltype(impl(
      std::make_index_sequence<sizeof...(T1s) * sizeof...(T2s)>{}));
};

template <class T1, class T2>
using type_product_t = typename type_product<T1, T2>::type;

/// @}

}  // namespace rigid_geometric_algebra::detail
