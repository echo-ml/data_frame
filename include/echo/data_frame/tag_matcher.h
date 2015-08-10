#pragma once

#include <echo/htl.h>

namespace echo {
namespace data_frame {
namespace detail {
// Functor class to work around bug with lambda expressions in intel compiler
template <class Tag1>
struct TagMatcher {
  template <class Tag2>
  auto operator()(Tag2) const
      -> htl::integral_constant<bool, std::is_same<Tag1, Tag2>::value> {
    return {};
  }
};
}
}
}
