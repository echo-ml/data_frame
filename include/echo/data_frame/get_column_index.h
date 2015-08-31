#pragma once

#define DETAIL_NS detail_get_column_index

#include <echo/data_frame/concept.h>
#include <echo/htl.h>

namespace echo {
namespace data_frame {
//------------------------------------------------------------------------------
// TagMatcher
//------------------------------------------------------------------------------
namespace DETAIL_NS {
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

//------------------------------------------------------------------------------
// get_column_index
//------------------------------------------------------------------------------
template <class ColumnTags, class ColumnTag,
          CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
auto get_column_index() {
  return htl::find_if(DETAIL_NS::TagMatcher<ColumnTag>(), ColumnTags());
}
}
}

#undef DETAIL_NS
