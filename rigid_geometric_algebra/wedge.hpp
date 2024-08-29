#pragma once

namespace rigid_geometric_algebra {

inline constexpr class
{
public:
  template <class T1, class T2>
  constexpr auto
  operator()(const T1& t1, const T2& t2) const -> decltype(t1 ^ t2)
  {
    return t1 ^ t2;
  }
} wedge{};

}  // namespace rigid_geometric_algebra
